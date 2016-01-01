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
#include "HandlerFunctions.h"
#include "Character\Buff.h"
#include "Gameplay\Stage.h"
#include "IO\UI.h"
#include "IO\UITypes\UIEquipInventory.h"
#include "IO\UITypes\UIItemInventory.h"

namespace Net
{
	using Character::Player;
	using Character::Buff;

	void ChangeStatsHandler::handle(InPacket& recv) const
	{
		recv.readbool(); // 'itemreaction'

		int32_t updatemask = recv.readint();
		if (updatemask == 0)
		{
			IO::UI::enable();
			return;
		}

		Player& player = Gameplay::Stage::getplayer();

		for (size_t i = 0; i < 20; i++)
		{
			Maplestat stat = Character::statvalues[i];

			if (updatemask & stat)
			{
				switch (stat)
				{
				case Character::MS_SKIN:
					player.getlook().setbody(static_cast<int8_t>(recv.readshort()));
					break;
				case Character::MS_FACE:
					player.getlook().setface(recv.readint());
					break;
				case Character::MS_HAIR:
					player.getlook().sethair(recv.readint());
					break;
				case Character::MS_LEVEL:
					player.getstats().setstat(stat, static_cast<uint8_t>(recv.readbyte()));
					//parent.getstage().showchareffect(0);
					break;
				case Character::MS_EXP:
					player.getstats().setexp(recv.readint());
					break;
				case Character::MS_MESO:
					player.getinvent().setmeso(recv.readint());
					break;
				case Character::MS_AP:
					player.getstats().setstat(stat, recv.readshort());
					/*if (uinterface.getelement(UI_STATSINFO))
					{
					uinterface.getelement(UI_STATSINFO)->sendbool(player->getstats()->getstat(MS_AP) > 0);
					}*/
					break;
				default:
					player.getstats().setstat(stat, recv.readshort());
					player.recalcstats(false);
					break;
				}
			}
		}
	}

	void RecalculateStatsHandler::handle(InPacket&) const
	{
		Gameplay::Stage::getplayer().recalcstats(false);
	}

	void UpdateskillsHandler::handle(InPacket& recv) const
	{
		recv.skip(3);

		int32_t skillid = recv.readint();
		int32_t level = recv.readint();
		int32_t masterlevel = recv.readint();
		int64_t expire = recv.readlong();

		Gameplay::Stage::getplayer().getskills().setskill(skillid, level, masterlevel, expire);
	}

	void ApplyBuffHandler::handle(InPacket& recv) const
	{
		int64_t firstmask = recv.readlong();
		int64_t secondmask = recv.readlong();

		Player& player = Gameplay::Stage::getplayer();

		vector<Buffstat> newbuffs;
		for (size_t i = 0; i < Character::FIRST_BUFFS; i++)
		{
			Buffstat buffvalue = Character::firstbuffs[i];
			if (firstmask & buffvalue)
				handlebuff(recv, buffvalue);
		}
		for (size_t i = 0; i < Character::SECOND_BUFFS; i++)
		{
			Buffstat buffvalue = Character::secondbuffs[i];
			if (secondmask & buffvalue)
				handlebuff(recv, buffvalue);
		}

		player.recalcstats(false);
	}

	void ApplyBuffHandler::handlebuff(InPacket& recv, Buffstat bs) const
	{
		Buff buff;
		buff.stat = bs;
		buff.value = recv.readshort();
		buff.skillid = recv.readint();
		buff.duration = recv.readint();

		IO::UI::showbuff(buff.skillid, buff.duration);
		Gameplay::Stage::getplayer().givebuff(buff);
	}
}