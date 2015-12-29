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
#include "PlayerHandlers83.h"
#include "Gameplay\Stage.h"
#include "IO\UI.h"
#include "IO\UITypes\UIEquipInventory.h"
#include "IO\UITypes\UIItemInventory.h"

namespace Net
{
	void StatschangedHandler83::handle(InPacket& recv) const
	{
		recv.readbool(); // 'itemreaction'

		int32_t updatemask = recv.readint();
		if (updatemask == 0)
		{
			IO::UI::enable();
			return;
		}

		for (size_t i = 0; i < 20; i++)
		{
			using Character::Player;
			Player& player = Gameplay::Stage::getplayer();
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

	void StatresetHandler83::handle(InPacket&) const
	{
		Gameplay::Stage::getplayer().recalcstats(false);
	}

	void UpdateskillsHandler83::handle(InPacket& recv) const
	{
		recv.skip(3);

		int32_t skillid = recv.readint();
		int32_t level = recv.readint();
		int32_t masterlevel = recv.readint();
		int64_t expire = recv.readlong();

		Gameplay::Stage::getplayer().getskills().setskill(skillid, level, masterlevel, expire);
	}

	void ModifyInventoryHandler::handle(InPacket& recv) const
	{
		recv.readbool(); // 'updatetick'

		int8_t size = recv.readbyte();
		for (int8_t i = 0; i < size; i++)
		{
			int8_t mode = recv.readbyte();
			Inventory::InvType invtype = static_cast<Inventory::InvType>(recv.readbyte());
			int16_t pos = recv.readshort();
			int16_t arg = (mode == 1 || mode == 2) ? recv.readshort() : 0;

			Inventory::Movetype move;
			if ((mode == 2 && (pos < 0 || arg < 0)) || (mode == 3 && pos < 0))
				move = static_cast<Inventory::Movetype>(recv.readbyte());
			else
				move = Inventory::MOVE_INTERNAL;

			Inventory& inventory = Gameplay::Stage::getplayer().getinvent();
			if (mode == 0)
				parseitem(recv, invtype, pos, inventory);
			else
				inventory.modify(invtype, pos, mode, arg, move);

			using IO::Element;
			using IO::UIEquipInventory;
			using IO::UIItemInventory;

			UIEquipInventory* eqinvent = IO::UI::getelement<UIEquipInventory>(Element::EQUIPINVENTORY);
			UIItemInventory* itinvent = IO::UI::getelement<UIItemInventory>(Element::ITEMINVENTORY);

			bool equipchanged = false;
			switch (move)
			{
			case Inventory::MOVE_INTERNAL:
				switch (invtype)
				{
				case Inventory::EQUIPPED:
					if (eqinvent)
						eqinvent->modify(pos, mode, arg);
					equipchanged = true;
					break;
				case Inventory::EQUIP:
				case Inventory::USE:
				case Inventory::SETUP:
				case Inventory::ETC:
				case Inventory::CASH:
					if (itinvent)
						itinvent->modify(invtype, pos, mode, arg);
					break;
				}
				break;
			case Inventory::MOVE_EQUIP:
			case Inventory::MOVE_UNEQUIP:
				if (pos < 0)
				{
					if (eqinvent)
						eqinvent->modify(-pos, 3, 0);

					if (itinvent)
						itinvent->modify(Inventory::EQUIP, arg, 0, 0);

					Gameplay::Stage::getplayer().changecloth(-pos);
				}
				else if (arg < 0)
				{
					if (eqinvent)
						eqinvent->modify(-arg, 0, 0);

					if (itinvent)
						itinvent->modify(Inventory::EQUIP, pos, 3, 0);

					Gameplay::Stage::getplayer().changecloth(-arg);
				}
				break;
			}

			IO::UI::enable();
		}
	}
}