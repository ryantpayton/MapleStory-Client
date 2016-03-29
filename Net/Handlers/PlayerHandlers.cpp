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
#include "Character\Buff.h"
#include "Gameplay\Stage.h"
#include "IO\UI.h"
#include "IO\UITypes\UIBuffList.h"
#include "IO\UITypes\UIStatsinfo.h"

namespace Net
{
	using Character::Char;
	using Character::Player;
	using Character::Buff;
	using Character::Maplestat;
	using Gameplay::Stage;
	using IO::UI;
	using IO::UIElement;
	using IO::UIBuffList;
	using IO::UIStatsinfo;

	void KeymapHandler::handle(InPacket& recv) const
	{
		recv.skip(1);

		for (uint8_t i = 0; i < 90; i++)
		{
			uint8_t type = recv.readbyte();
			int32_t action = recv.readint();

			UI::get().addkeymapping(i, type, action);
		}
	}

	void SkillMacrosHandler::handle(InPacket& recv) const
	{
		uint8_t size = recv.readbyte();
		for (uint8_t i = 0; i < size; i++)
		{
			recv.read<string>(); // name
			recv.readbyte(); // 'shout' byte
			recv.readint(); // skill 1
			recv.readint(); // skill 2
			recv.readint(); // skill 3
		}
	}

	void ChangeStatsHandler::handle(InPacket& recv) const
	{
		recv.readbool(); // 'itemreaction'

		int32_t updatemask = recv.readint();

		Player& player = Stage::get().getplayer();

		for (auto it = Maplestat::it(); it.hasnext(); it.increment())
		{
			Maplestat::Value stat = it.get();
			if (Maplestat::compare(stat, updatemask))
			{
				switch (stat)
				{
				case Maplestat::SKIN:
					player.changelook(stat, recv.readshort());
					break;
				case Maplestat::FACE:
				case Maplestat::HAIR:
					player.changelook(stat, recv.readint());
					break;
				case Maplestat::LEVEL:
					player.changelevel(recv.readbyte());
					break;
				case Maplestat::JOB:
					player.changejob(recv.readshort());
					break;
				case Maplestat::EXP:
					player.getstats().setexp(recv.readint());
					break;
				case Maplestat::MESO:
					player.getinvent().setmeso(recv.readint());
					break;
				default:
					player.getstats().setstat(stat, recv.readshort());
					player.recalcstats(false);
					break;
				}
			}
		}

		UI::get().enable();
	}

	void RecalculateStatsHandler::handle(InPacket&) const
	{
		Stage::get().getplayer().recalcstats(false);
	}

	void UpdateskillsHandler::handle(InPacket& recv) const
	{
		recv.skip(3);

		int32_t skillid = recv.readint();
		int32_t level = recv.readint();
		int32_t masterlevel = recv.readint();
		int64_t expire = recv.readlong();

		Stage::get().getplayer().getskills().setskill(skillid, level, masterlevel, expire);
	}

	void BuffHandler::handle(InPacket& recv) const
	{
		int64_t firstmask = recv.readlong();
		int64_t secondmask = recv.readlong();

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
		int16_t value = recv.readshort();
		int32_t skillid = recv.readint();
		int32_t duration = recv.readint();

		Buff buff = Buff(bs, value, skillid, duration);
		Stage::get().getplayer().givebuff(buff);

		UI::get().withelement(UIElement::BUFFLIST, &UIBuffList::addbuff, skillid, duration);
	}

	void CancelBuffHandler::handlebuff(InPacket&, Buffstat::Value bs) const
	{
		Stage::get().getplayer().cancelbuff(bs);
	}
}