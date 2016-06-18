/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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

#include "..\..\Character\Inventory\Inventory.h"
#include "..\..\Gameplay\Stage.h"
#include "..\..\IO\UI.h"
#include "..\..\IO\Messages.h"
#include "..\..\IO\UITypes\UIShop.h"
#include "..\..\IO\UITypes\UIEquipInventory.h"
#include "..\..\IO\UITypes\UIItemInventory.h"

namespace jrc
{
	void GatherResultHandler::handle(InPacket&) const
	{
		UI::get().get_element<UIItemInventory>()
			.if_present(&UIItemInventory::enable_sort);
	}


	void SortResultHandler::handle(InPacket&) const
	{
		UI::get().get_element<UIItemInventory>()
			.if_present(&UIItemInventory::enable_gather);
	}


	void ModifyInventoryHandler::handle(InPacket& recv) const
	{
		recv.read_bool(); // 'updatetick'

		Inventory& inventory = Stage::get().get_player().get_inventory();

		struct Mod
		{
			int8_t mode;
			Inventory::Type type;
			int16_t pos;
			int16_t arg;
		};
		std::vector<Mod> mods;

		int8_t size = recv.read_byte();
		for (int8_t i = 0; i < size; i++)
		{
			Mod mod;
			mod.mode = recv.read_byte();
			mod.type = Inventory::typebyvalue(recv.read_byte());
			mod.pos = recv.read_short();

			mod.arg = 0;
			switch (mod.mode)
			{
			case 0:
				ItemParser::parse_item(recv, mod.type, mod.pos, inventory);
				break;
			case 1:
				mod.arg = recv.read_short();
				if (inventory.change_count(mod.type, mod.pos, mod.arg))
					mod.mode = 4;
				break;
			case 2:
				mod.arg = recv.read_short();
				break;
			case 3:
				inventory.modify(mod.type, mod.pos, mod.mode, mod.arg, Inventory::MOVE_INTERNAL);
				break;
			}

			mods.push_back(mod);
		}

		Inventory::Movement move = (recv.length() > 0) ?
			Inventory::movementbyvalue(recv.read_byte()) :
			Inventory::MOVE_INTERNAL;

		for (Mod& mod : mods)
		{
			if (mod.mode == 2)
			{
				inventory.modify(mod.type, mod.pos, mod.mode, mod.arg, move);
			}

			UI::get().get_element<UIShop>()
				.if_present(&UIShop::modify, mod.type);

			auto eqinvent = UI::get().get_element<UIEquipInventory>();
			auto itinvent = UI::get().get_element<UIItemInventory>();
			switch (move)
			{
			case Inventory::MOVE_INTERNAL:
				switch (mod.type)
				{
				case Inventory::EQUIPPED:
					if (eqinvent)
						eqinvent->modify(mod.pos, mod.mode, mod.arg);

					Stage::get().get_player().change_equip(-mod.pos);
					Stage::get().get_player().change_equip(-mod.arg);
					break;
				case Inventory::EQUIP:
				case Inventory::USE:
				case Inventory::SETUP:
				case Inventory::ETC:
				case Inventory::CASH:
					if (itinvent)
						itinvent->modify(mod.type, mod.pos, mod.mode, mod.arg);
					break;
				}
				break;
			case Inventory::MOVE_EQUIP:
			case Inventory::MOVE_UNEQUIP:
				if (mod.pos < 0)
				{
					if (eqinvent)
						eqinvent->modify(-mod.pos, 3, 0);

					if (itinvent)
						itinvent->modify(Inventory::EQUIP, mod.arg, 0, 0);

					Stage::get().get_player().change_equip(-mod.pos);
				}
				else if (mod.arg < 0)
				{
					if (eqinvent)
						eqinvent->modify(-mod.arg, 0, 0);

					if (itinvent)
						itinvent->modify(Inventory::EQUIP, mod.pos, 3, 0);

					Stage::get().get_player().change_equip(-mod.arg);
				}
				break;
			}
		}

		Stage::get().get_player().recalc_stats(true);
		UI::get().enable();
	}
}