/////////////////////////////////////////////////////////////////////////////
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
#pragma once
#include "AbstractItemHandler83.h"
#include "IO\UITypes\UIStatusbar.h"
#include "IO\UI.h"
#include "IO\Window.h"
#include "Gameplay\Stage.h"
#include "Program\Configuration.h"
#include "Net\Session.h"
#include "Graphics\GraphicsEngine.h"

namespace Net
{
	using Character::Inventory;
	using Character::Skillbook;
	using Character::Questlog;
	using Character::Monsterbook;
	using Character::Telerock;

	void stagetransition(uint8_t portalid, int32_t mapid)
	{
		IO::Window::fadeout();
		Gameplay::Stage::clear();
		Graphics::GraphicsEngine::clear();
		Gameplay::Stage::setmap(portalid, mapid);
	}

	// Handler for a packet which contains all character information on first login.
	class CharacterInfoHandler : public AbstractItemHandler83
	{
		void handle(InPacket& recv) const override
		{
			int32_t cid = recv.readint();
			if (!Gameplay::Stage::loadplayer(cid))
				return;

			Character::Player& player = Gameplay::Stage::getplayer();
			recv.readbyte(); // 'buddycap'
			if (recv.readbool())
			{
				recv.readascii(); // 'linkedname'
			}
			parseinventory(recv, player.getinvent());
			parseskillbook(recv, player.getskills());
			parsequestlog(recv, player.getquests());
			parsetelerock(recv, player.gettrock());
			parsemonsterbook(recv, player.getmonsterbook());

			player.recalcstats(true);

			IO::UI::remove(IO::Element::CHARSELECT);
			IO::UI::remove(IO::Element::SOFTKEYBOARD);

			stagetransition(player.getstats().getportal(), player.getstats().getmapid());

			IO::UI::add(IO::ElementStatusbar(player.getstats()));
		}

		void parseinventory(InPacket& recv, Inventory& invent) const
		{
			invent.setmeso(recv.readint());
			invent.setslots(Inventory::EQUIP, recv.readbyte());
			invent.setslots(Inventory::USE, recv.readbyte());
			invent.setslots(Inventory::SETUP, recv.readbyte());
			invent.setslots(Inventory::ETC, recv.readbyte());
			invent.setslots(Inventory::CASH, recv.readbyte());

			for (size_t i = 0; i < 3; i++)
			{
				Inventory::InvType inv = (i == 0) ? Inventory::EQUIPPED : Inventory::EQUIP;
				int16_t pos = recv.readshort();
				while (pos != 0)
				{
					int16_t slot = (i == 1) ? -pos : pos;
					parseitem(recv, inv, slot, invent);
					pos = recv.readshort();
				}
			}
			recv.skip(2);

			Inventory::InvType toparse[4] =
			{ 
				Inventory::USE, Inventory::SETUP, Inventory::ETC, Inventory::CASH
			};

			for (size_t i = 0; i < 4; i++)
			{
				Inventory::InvType inv = toparse[i];
				int8_t pos = recv.readbyte();
				while (pos != 0)
				{
					parseitem(recv, inv, pos, invent);
					pos = recv.readbyte();
				}
			}
		}

		void parseskillbook(InPacket& recv, Skillbook& skills) const
		{
			int16_t size = recv.readshort();
			for (int16_t i = 0; i < size; i++)
			{
				int32_t skillid = recv.readint();
				int32_t level = recv.readint();
				int64_t expiration = recv.readlong();
				bool fourthtjob = ((skillid % 100000) / 10000 == 2);
				int32_t masterlevel = fourthtjob ? recv.readint() : -1;
				skills.setskill(skillid, level, masterlevel, expiration);
			}
			size = recv.readshort();
			for (int16_t i = 0; i < size; i++)
			{
				int32_t skillid = recv.readint();
				int32_t cooldown = recv.readshort();
				skills.setcd(skillid, cooldown);
			}
		}

		void parsequestlog(InPacket& recv, Questlog& quests) const
		{
			int16_t size = recv.readshort();
			for (int16_t i = 0; i < size; i++)
			{
				int16_t qid = recv.readshort();
				string qdata = recv.readascii();
				if (quests.isstarted(qid))
				{
					int16_t qidl = quests.getlaststarted();
					quests.addprogress(qidl, qid, qdata);
					i--;
				}
				else
				{
					quests.addstarted(qid, qdata);
				}
			}
			map<int16_t, int64_t> completed;
			size = recv.readshort();
			for (int16_t i = 0; i < size; i++)
			{
				int16_t qid = recv.readshort();
				int64_t time = recv.readlong();
				quests.addcompleted(qid, time);
			}
		}

		void parsemonsterbook(InPacket& recv, Monsterbook& monsterbook) const
		{
			int32_t cover = recv.readint();
			monsterbook.setcover(cover);
			recv.skip(1);
			int16_t size = recv.readshort();
			for (int16_t i = 0; i < size; i++)
			{
				int16_t cid = recv.readshort();
				int8_t mblv = recv.readbyte();
				monsterbook.addcard(cid, mblv);
			}
		}

		void parsetelerock(InPacket& recv, Telerock& trock) const
		{
			for (size_t i = 0; i < 5; i++)
			{
				int32_t mapid = recv.readint();
				trock.addlocation(mapid);
			}
			for (size_t i = 0; i < 10; i++)
			{
				int32_t mapid = recv.readint();
				trock.addviplocation(mapid);
			}
		}
	};

	// Handler for a packet which warps the player to a different map.
	class WarpToMapHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			// Spawn information.
			int32_t mapid = recv.readint();
			uint8_t portalid = static_cast<uint8_t>(recv.readint());
			stagetransition(portalid, mapid);
		}
	};

	// Handler for a packet which warps the player to a different channel.
	class ChangeChannelHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			// Spawn information.
			recv.readbyte(); // channel
			// ip adress etc.
		}
	};

	// Handler for a packet which contains a custom, server-sided map.
	class MapPacketHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			stagetransition(0, 0);
			Gameplay::Stage::parsemap(recv);
		}
	};
}