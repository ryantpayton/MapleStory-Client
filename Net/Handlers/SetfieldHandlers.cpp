//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "SetfieldHandlers.h"
#include "ItemParser.h"

#include "..\..\Configuration.h"
#include "..\..\Console.h"
#include "..\..\Constants.h"
#include "..\..\Timer.h"
#include "..\..\Audio\Audio.h"
#include "..\..\Gameplay\Stage.h"
#include "..\..\Graphics\GraphicsGL.h"
#include "..\..\IO\UI.h"
#include "..\..\IO\Window.h"
#include "..\..\Net\Session.h"

namespace jrc
{
	void stagetransition(uint8_t portalid, int32_t mapid)
	{
		float fadestep = 0.025f;
		Window::get().fadeout(fadestep, [](){
			GraphicsGL::get().clear();
			Stage::get().reload();
			UI::get().enable();
			Timer::get().start();
			GraphicsGL::get().unlock();
		});

		GraphicsGL::get().lock();
		Stage::get().clear();
		Timer::get().start();

		Stage::get().setmap(portalid, mapid);
	}

	void SetfieldHandler::handle(InPacket& recv) const
	{
		int32_t channel = recv.read_int();
		int8_t mode1 = recv.read_byte();
		int8_t mode2 = recv.read_byte();
		if (mode1 == 0 && mode2 == 0)
		{
			change_map(recv, channel);
		}
		else
		{
			set_field(recv);
		}
	}

	void SetfieldHandler::change_map(InPacket& recv, int32_t) const
	{
		recv.skip(3);

		int32_t mapid = recv.read_int();
		int8_t portalid = recv.read_byte();

		stagetransition(portalid, mapid);
	}

	void SetfieldHandler::set_field(InPacket& recv) const
	{
		recv.skip(23);

		int32_t cid = recv.read_int();

		const CharEntry& playerentry = Session::get().getlogin().getcharbyid(cid);
		if (playerentry.cid != cid)
		{
			Console::get().print("Nonexisting cid: " + std::to_string(cid));
		}

		Stage::get().loadplayer(playerentry);

		Session::get().getlogin().parsestats(recv);

		Player& player = Stage::get().getplayer();

		recv.read_byte(); // 'buddycap'
		if (recv.read_bool())
		{
			recv.read_string(); // 'linkedname'
		}

		parse_inventory(recv, player.getinvent());
		parse_skillbook(recv, player.getskills());
		parse_questlog(recv, player.getquests());
		parseminigame(recv);
		parsering1(recv);
		parsering2(recv);
		parsering3(recv);
		parsetelerock(recv, player.gettrock());
		parsemonsterbook(recv, player.getmonsterbook());
		parsenewyear(recv);
		parseareainfo(recv);

		recv.skip(10);

		player.recalcstats(true);

		stagetransition(player.getstats().getportal(), player.getstats().getmapid());

		Sound(Sound::GAMESTART).play();

		UI::get().changestate(UI::GAME);
	}

	void SetfieldHandler::parse_inventory(InPacket& recv, Inventory& invent) const
	{
		invent.setmeso(recv.read_int());
		invent.setslots(Inventory::EQUIP, recv.read_byte());
		invent.setslots(Inventory::USE, recv.read_byte());
		invent.setslots(Inventory::SETUP, recv.read_byte());
		invent.setslots(Inventory::ETC, recv.read_byte());
		invent.setslots(Inventory::CASH, recv.read_byte());

		recv.skip(8);

		for (size_t i = 0; i < 3; i++)
		{
			Inventory::Type inv = (i == 0) ? Inventory::EQUIPPED : Inventory::EQUIP;
			int16_t pos = recv.read_short();
			while (pos != 0)
			{
				int16_t slot = (i == 1) ? -pos : pos;
				ItemParser::parseitem(recv, inv, slot, invent);
				pos = recv.read_short();
			}
		}

		recv.skip(2);

		Inventory::Type toparse[4] =
		{
			Inventory::USE, Inventory::SETUP, Inventory::ETC, Inventory::CASH
		};

		for (size_t i = 0; i < 4; i++)
		{
			Inventory::Type inv = toparse[i];
			int8_t pos = recv.read_byte();
			while (pos != 0)
			{
				ItemParser::parseitem(recv, inv, pos, invent);
				pos = recv.read_byte();
			}
		}
	}

	void SetfieldHandler::parse_skillbook(InPacket& recv, Skillbook& skills) const
	{
		int16_t size = recv.read_short();
		for (int16_t i = 0; i < size; i++)
		{
			int32_t skillid = recv.read_int();
			int32_t level = recv.read_int();
			int64_t expiration = recv.read_long();
			bool fourthtjob = ((skillid % 100000) / 10000 == 2);
			int32_t masterlevel = fourthtjob ? recv.read_int() : -1;
			skills.set_skill(skillid, level, masterlevel, expiration);
		}

		size = recv.read_short();
		for (int16_t i = 0; i < size; i++)
		{
			int32_t skillid = recv.read_int();
			int32_t cooldown = recv.read_short();
			skills.set_cd(skillid, cooldown);
		}
	}

	void SetfieldHandler::parse_questlog(InPacket& recv, Questlog& quests) const
	{
		int16_t size = recv.read_short();
		for (int16_t i = 0; i < size; i++)
		{
			int16_t qid = recv.read_short();
			std::string qdata = recv.read_string();
			if (quests.is_started(qid))
			{
				int16_t qidl = quests.get_last_started();
				quests.add_in_progress(qidl, qid, qdata);
				i--;
			}
			else
			{
				quests.add_started(qid, qdata);
			}
		}

		std::map<int16_t, int64_t> completed;
		size = recv.read_short();
		for (int16_t i = 0; i < size; i++)
		{
			int16_t qid = recv.read_short();
			int64_t time = recv.read_long();
			quests.add_completed(qid, time);
		}
	}

	void SetfieldHandler::parsering1(InPacket& recv) const
	{
		int16_t rsize = recv.read_short();
		for (int16_t i = 0; i < rsize; i++)
		{
			recv.read_int();
			recv.read_padded_string(13);
			recv.read_int();
			recv.read_int();
			recv.read_int();
			recv.read_int();
		}
	}

	void SetfieldHandler::parsering2(InPacket& recv) const
	{
		int16_t rsize = recv.read_short();
		for (int16_t i = 0; i < rsize; i++)
		{
			recv.read_int();
			recv.read_padded_string(13);
			recv.read_int();
			recv.read_int();
			recv.read_int();
			recv.read_int();
			recv.read_int();
		}
	}

	void SetfieldHandler::parsering3(InPacket& recv) const
	{
		int16_t rsize = recv.read_short();
		for (int16_t i = 0; i < rsize; i++)
		{
			recv.read_int();
			recv.read_int();
			recv.read_int();
			recv.read_short();
			recv.read_int();
			recv.read_int();
			recv.read_padded_string(13);
			recv.read_padded_string(13);
		}
	}

	void SetfieldHandler::parseminigame(InPacket& recv) const
	{
		int16_t mgsize = recv.read_short();
		for (int16_t i = 0; i < mgsize; i++)
		{
		}
	}

	void SetfieldHandler::parsemonsterbook(InPacket& recv, Monsterbook& monsterbook) const
	{
		monsterbook.setcover(recv.read_int());

		recv.skip(1);

		int16_t size = recv.read_short();
		for (int16_t i = 0; i < size; i++)
		{
			int16_t cid = recv.read_short();
			int8_t mblv = recv.read_byte();

			monsterbook.addcard(cid, mblv);
		}
	}

	void SetfieldHandler::parsetelerock(InPacket& recv, Telerock& trock) const
	{
		for (size_t i = 0; i < 5; i++)
		{
			trock.addlocation(recv.read_int());
		}

		for (size_t i = 0; i < 10; i++)
		{
			trock.addviplocation(recv.read_int());
		}
	}

	void SetfieldHandler::parsenewyear(InPacket& recv) const
	{
		int16_t nysize = recv.read_short();
		for (int16_t i = 0; i < nysize; i++)
		{
		}
	}

	void SetfieldHandler::parseareainfo(InPacket& recv) const
	{
		std::map<int16_t, std::string> areainfo;
		int16_t arsize = recv.read_short();
		for (int16_t i = 0; i < arsize; i++)
		{
			int16_t area = recv.read_short();
			areainfo[area] = recv.read_string();
		}
	}
}