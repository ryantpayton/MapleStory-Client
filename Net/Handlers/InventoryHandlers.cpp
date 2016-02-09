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
#include "InventoryHandlers.h"
#include "ItemParser.h"

#include "IO\UI.h"
#include "IO\UITypes\UIEquipInventory.h"
#include "IO\UITypes\UIItemInventory.h"
#include "Gameplay\Stage.h"
#include "Character\Inventory\Inventory.h"

namespace Net
{
	using Character::Inventory; 
	using Gameplay::Stage;
	using IO::UI;
	using IO::Element;
	using IO::UIEquipInventory;
	using IO::UIItemInventory;

	void GatherResultHandler::handle(InPacket&) const
	{
		UIItemInventory* itinvent = UI::get().getelement<UIItemInventory>(Element::ITEMINVENTORY);

		if (itinvent)
			itinvent->enablesort();
	}

	void SortResultHandler::handle(InPacket&) const
	{
		UIItemInventory* itinvent = UI::get().getelement<UIItemInventory>(Element::ITEMINVENTORY);

		if (itinvent)
			itinvent->enablegather();
	}

	void ModifyInventoryHandler::handle(InPacket& recv) const
	{
		recv.readbool(); // 'updatetick'

		Inventory& inventory = Stage::get().getplayer().getinvent();

		int8_t size = recv.readbyte();
		for (int8_t i = 0; i < size; i++)
		{
			int8_t mode = recv.readbyte();
			Inventory::Type invtype = Inventory::typebyvalue(recv.readbyte());
			int16_t pos = recv.readshort();
			int16_t arg = (mode == 1 || mode == 2) ? recv.readshort() : 0;

			Inventory::Movement move;
			if ((mode == 2 && (pos < 0 || arg < 0)) || (mode == 3 && pos < 0))
				move = Inventory::movementbyvalue(recv.readbyte());
			else
				move = Inventory::MOVE_INTERNAL;

			if (mode == 0)
				ItemParser::parseitem(recv, invtype, pos, inventory);
			else
				inventory.modify(invtype, pos, mode, arg, move);

			UIEquipInventory* eqinvent = UI::get().getelement<UIEquipInventory>(Element::EQUIPINVENTORY);
			UIItemInventory* itinvent = UI::get().getelement<UIItemInventory>(Element::ITEMINVENTORY);

			switch (move)
			{
			case Inventory::MOVE_INTERNAL:
				switch (invtype)
				{
				case Inventory::EQUIPPED:
					if (eqinvent)
						eqinvent->modify(pos, mode, arg);

					Stage::get().getplayer().changecloth(-pos);
					Stage::get().getplayer().changecloth(-arg);
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

					Stage::get().getplayer().changecloth(-pos);
				}
				else if (arg < 0)
				{
					if (eqinvent)
						eqinvent->modify(-arg, 0, 0);

					if (itinvent)
						itinvent->modify(Inventory::EQUIP, pos, 3, 0);

					Stage::get().getplayer().changecloth(-arg);
				}
				break;
			}
		}


		UI::get().enable();
	}
}