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
#include "PlayerHandlers.h"

#include "..\..\Timer.h"
#include "..\..\Character\Buff.h"
#include "..\..\Gameplay\Stage.h"
#include "..\..\IO\UI.h"
#include "..\..\IO\UITypes\UIBuffList.h"
#include "..\..\IO\UITypes\UIStatsinfo.h"

namespace jrc
{
	void KeymapHandler::handle(InPacket& recv) const
	{
		recv.skip(1);

		for (uint8_t i = 0; i < 90; i++)
		{
			uint8_t type = recv.read_byte();
			int32_t action = recv.read_int();

			UI::get().addkeymapping(i, type, action);
		}
	}


	void SkillMacrosHandler::handle(InPacket& recv) const
	{
		uint8_t size = recv.read_byte();
		for (uint8_t i = 0; i < size; i++)
		{
			recv.read_string(); // name
			recv.read_byte(); // 'shout' byte
			recv.read_int(); // skill 1
			recv.read_int(); // skill 2
			recv.read_int(); // skill 3
		}
	}


	void ChangeStatsHandler::handle(InPacket& recv) const
	{
		recv.read_bool(); // 'itemreaction'

		int32_t updatemask = recv.read_int();

		Player& player = Stage::get().getplayer();

		bool recalculate = false;
		for (auto it = Maplestat::it(); it.hasnext(); it.increment())
		{
			Maplestat::Value stat = it.get();
			if (Maplestat::compare(stat, updatemask))
			{
				bool updatesingle = false;
				switch (stat)
				{
				case Maplestat::SKIN:
					player.changelook(stat, recv.read_short());
					break;
				case Maplestat::FACE:
				case Maplestat::HAIR:
					player.changelook(stat, recv.read_int());
					break;
				case Maplestat::LEVEL:
					player.changelevel(recv.read_byte());
					updatesingle = true;
					break;
				case Maplestat::JOB:
					player.changejob(recv.read_short());
					updatesingle = true;
					break;
				case Maplestat::EXP:
					player.getstats().setexp(recv.read_int());
					break;
				case Maplestat::MESO:
					player.getinvent().setmeso(recv.read_int());
					break;
				default:
					player.getstats().setstat(stat, recv.read_short());
					recalculate = true;
					break;
				}

				if (updatesingle)
				{
					UI::get().getelement(UIElement::STATSINFO)
						.reinterpret<UIStatsinfo>()
						->updatestat(stat);
				}
			}
		}

		if (recalculate)
		{
			player.recalcstats(false);
		}

		UI::get().enable();
	}


	void BuffHandler::handle(InPacket& recv) const
	{
		int64_t firstmask = recv.read_long();
		int64_t secondmask = recv.read_long();

		switch (secondmask)
		{
		case Buffstat::BATTLESHIP:
			handlebuff(recv, Buffstat::BATTLESHIP);
			return;
		}

		for (size_t i = 0; i < Buffstat::FIRST_BUFFS; i++)
		{
			Buffstat::Value buffvalue = Buffstat::firstbyid(i);
			if (firstmask & buffvalue)
				handlebuff(recv, buffvalue);
		}
		for (size_t i = 0; i < Buffstat::SECOND_BUFFS; i++)
		{
			Buffstat::Value buffvalue = Buffstat::secondbyid(i);
			if (secondmask & buffvalue)
				handlebuff(recv, buffvalue);
		}

		Stage::get().getplayer().recalcstats(false);
	}

	void ApplyBuffHandler::handlebuff(InPacket& recv, Buffstat::Value bs) const
	{
		int16_t value = recv.read_short();
		int32_t skillid = recv.read_int();
		int32_t duration = recv.read_int();

		Buff buff(bs, value, skillid, duration);
		Stage::get().getplayer().givebuff(buff);

		UI::get().getelement(UIElement::BUFFLIST)
			.reinterpret<UIBuffList>()
			->addbuff(skillid, duration);
	}

	void CancelBuffHandler::handlebuff(InPacket&, Buffstat::Value bs) const
	{
		Stage::get().getplayer().cancelbuff(bs);
	}


	void RecalculateStatsHandler::handle(InPacket&) const
	{
		Stage::get().getplayer().recalcstats(false);
	}


	void UpdateskillsHandler::handle(InPacket& recv) const
	{
		recv.skip(3);

		int32_t skillid = recv.read_int();
		int32_t level = recv.read_int();
		int32_t masterlevel = recv.read_int();
		int64_t expire = recv.read_long();

		Stage::get().getplayer().getskills().set_skill(skillid, level, masterlevel, expire);
	}


	void AddCooldownHandler::handle(InPacket& recv) const
	{
		int32_t skillid = recv.read_int();
		int16_t time = recv.read_short();

		int32_t seconds = Timer::get().seconds() + time;

		Stage::get().getplayer().getskills().set_cd(skillid, seconds);
	}
}