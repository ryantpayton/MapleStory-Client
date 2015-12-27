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
#include "Gameplay\Stage.h"
#include "IO\UI.h"
#include "IO\UITypes\UIEquipInventory.h"

namespace Net
{
	// Handles a packet which notifies the client of changes in character stats.
	class StatschangedHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
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
	};

	// Handles a packet which forces a stats recalculation.
	class StatresetHandler83 : public PacketHandler
	{
		void handle(InPacket&) const override
		{
			Gameplay::Stage::getplayer().recalcstats(false);
		}
	};

	// Handles a packet which notifies the client of changes in a character's skills.
	class UpdateskillsHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			recv.skip(3);

			int32_t skillid = recv.readint();
			int32_t level = recv.readint();
			int32_t masterlevel = recv.readint();
			int64_t expire = recv.readlong();

			Gameplay::Stage::getplayer().getskills().setskill(skillid, level, masterlevel, expire);
		}
	};

	// Handler for a packet which modifies the player's inventory.
	class ModifyInventoryHandler : public AbstractItemHandler83
	{
		void handle(InPacket& recv) const override
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
				UIEquipInventory* eqinvent = IO::UI::getelement<UIEquipInventory>(Element::EQUIPINVENTORY);
				//UIItemInventory* itinvent = uinterface.getelement<iteminventory>(UI_INVENTORY);

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
					case Inventory::USE:
					case Inventory::SETUP:
					case Inventory::ETC:
					case Inventory::CASH:
						//itinvent->modify(invtype, pos, mode, arg);
						break;
					}
					break;
				case Inventory::MOVE_UNEQUIP:
					if (pos < 0)
					{
						if (eqinvent)
							eqinvent->modify(-pos, 3, 0);
						//itinvent->modify(IVT_EQUIP, arg, 0, 0);
						equipchanged = true;
					}
					break;
				case Inventory::MOVE_EQUIP:
					if (arg < 0)
					{
						if (eqinvent)
							eqinvent->modify(-arg, 0, 0);
						//itinvent->modify(IVT_EQUIP, pos, 3, 0);
						equipchanged = true;
					}
					break;
				}

				if (equipchanged)
				{
					using Character::Player;
					Player& player = Gameplay::Stage::getplayer();
					//player.equipchanged((move == Inventory::MOVE_EQUIP) ? -arg : -pos);
					player.recalcstats(true);
				}

				IO::UI::enable();
			}
		}
	};
}