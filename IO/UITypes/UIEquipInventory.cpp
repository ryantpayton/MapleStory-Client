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
#include "UIEquipInventory.h"

#include "..\UI.h"
#include "..\Components\MapleButton.h"

#include "..\..\Gameplay\Stage.h"
#include "..\..\Net\Packets\InventoryPackets.h"

#include "nlnx\nx.hpp"

namespace jrc
{
	UIEquipInventory::UIEquipInventory() :
		UIDragElement<PosEQINV>(Point<int16_t>(184, 20)), inventory(Stage::get().get_player().get_inventory()) {

		iconpositions[1] = Point<int16_t>(43, 25);
		iconpositions[2] = Point<int16_t>(43, 91);
		iconpositions[3] = Point<int16_t>(43, 68);
		iconpositions[4] = Point<int16_t>(109, 91);
		iconpositions[5] = Point<int16_t>(43, 124);
		iconpositions[6] = Point<int16_t>(43, 157);
		iconpositions[7] = Point<int16_t>(76, 190);
		iconpositions[8] = Point<int16_t>(10, 157);
		iconpositions[9] = Point<int16_t>(142, 124);
		iconpositions[10] = Point<int16_t>(142, 124);
		iconpositions[11] = Point<int16_t>(109, 124);
		iconpositions[12] = Point<int16_t>(109, 157);
		iconpositions[13] = Point<int16_t>(142, 157);
		iconpositions[15] = Point<int16_t>(109, 91);
		iconpositions[16] = Point<int16_t>(142, 91);
		iconpositions[17] = Point<int16_t>(76, 124);
		iconpositions[18] = Point<int16_t>(142, 91);
		iconpositions[19] = Point<int16_t>(76, 124);
		iconpositions[49] = Point<int16_t>(10, 58);
		iconpositions[50] = Point<int16_t>(76, 157);

		nl::node source = nl::nx::ui["UIWindow2.img"]["Equip"]["character"];
		nl::node petsource = nl::nx::ui["UIWindow2.img"]["Equip"]["pet"];

		sprites.emplace_back(source["backgrnd"]);
		sprites.emplace_back(source["backgrnd2"]);
		sprites.emplace_back(source["backgrnd3_Kanna"]);
		sprites.emplace_back(source["cashPendant"]);
		sprites.emplace_back(source["charmPocket"]);
		sprites.emplace_back(source["emblem"]);

		buttons[BT_TOGGLEPETS] = std::make_unique<MapleButton>(source["BtPet"]);

		textures_pet.emplace_back(petsource["backgrnd"]);
		textures_pet.emplace_back(petsource["backgrnd2"]);
		textures_pet.emplace_back(petsource["backgrnd3"]);

		load_icons();

		dimension = { 184, 290 };
		active = true;
		showpetequips = false;
	}

	void UIEquipInventory::draw(float alpha) const
	{
		UIElement::draw(alpha);

		for (auto& icit : icons)
		{
			int16_t slot = icit.first;
			if (icit.second && iconpositions.count(slot))
				icit.second->draw(position + iconpositions.at(slot));
		}

		if (showpetequips)
		{
			Point<int16_t> position_pet(position + Point<int16_t>(184, 0));
			for (auto& texture : textures_pet)
			{
				texture.draw(position_pet);
			}
		}
	}

	Button::State UIEquipInventory::button_pressed(uint16_t id)
	{
		switch (id)
		{
		case BT_TOGGLEPETS:
			showpetequips = !showpetequips;
			return Button::NORMAL;
		default:
			return Button::PRESSED;
		}
	}

	void UIEquipInventory::update_slot(int16_t slot)
	{
		Optional<const Texture> texture = inventory.get_item(Inventory::EQUIPPED, slot)
			.transform(&Item::getidata)
			.transform(&ItemData::geticon, false);

		if (texture)
		{
			icons[slot] = std::make_unique<Icon>(
				std::make_unique<EquipIcon>(slot), 
				*texture, 
				-1
				);
		}
		else if (icons.count(slot))
		{
			icons.erase(slot);
		}

		clear_tooltip();
	}

	void UIEquipInventory::load_icons()
	{
		icons.clear();

		for (auto& icon : iconpositions)
		{
			int16_t slot = icon.first;
			update_slot(slot);
		}
	}

	Cursor::State UIEquipInventory::send_cursor(bool pressed, Point<int16_t> cursorpos)
	{
		Cursor::State dstate = UIDragElement::send_cursor(pressed, cursorpos);
		if (dragged)
		{
			clear_tooltip();
			return dstate;
		}

		int16_t slot = slot_by_position(cursorpos);
		Optional<Icon> icon = geticon(slot);
		if (icon)
		{
			if (pressed)
			{
				icon->start_drag(cursorpos - position - iconpositions[slot]);
				UI::get().drag_icon(icon.get());

				clear_tooltip();
				return Cursor::GRABBING;
			}
			else
			{
				show_equip(slot);
				return Cursor::CANGRAB;
			}
		}
		else
		{
			clear_tooltip();
			return Cursor::IDLE;
		}
	}

	void UIEquipInventory::doubleclick(Point<int16_t> cursorpos)
	{
		int16_t slot = slot_by_position(cursorpos);
		if (icons.count(slot))
		{
			int16_t freeslot = inventory.find_free_slot(Inventory::EQUIP);
			if (freeslot > 0)
			{
				UnequipItemPacket(slot, freeslot)
					.dispatch();
			}
		}
	}

	void UIEquipInventory::send_icon(const Icon& icon, Point<int16_t> cursorpos)
	{
		int16_t slot = slot_by_position(cursorpos);
		if (slot > 0)
		{
			Equipslot::Value eqslot = Equipslot::byvalue(slot);
			icon.drop_on_equips(eqslot);
		}
	}

	void UIEquipInventory::toggle_active()
	{
		clear_tooltip();
		UIElement::toggle_active();
	}

	void UIEquipInventory::modify(int16_t pos, int8_t mode, int16_t arg)
	{
		switch (mode)
		{
		case 0:
		case 3:
			update_slot(pos);
			break;
		case 2:
			update_slot(pos);
			update_slot(arg);
			break;
		}
	}

	void UIEquipInventory::show_equip(int16_t slot)
	{
		Optional<Equip> equip = inventory.get_equip(Inventory::EQUIPPED, slot);
		int16_t eqslot = -slot;
		UI::get().show_equip(TYPE, equip.get(), eqslot);
	}

	void UIEquipInventory::clear_tooltip()
	{
		UI::get().clear_tooltip(TYPE);
	}

	int16_t UIEquipInventory::slot_by_position(Point<int16_t> cursorpos) const
	{
		for (auto& icit : iconpositions)
		{
			int16_t slot = icit.first;
			Rectangle<int16_t> iconrect = Rectangle<int16_t>(
				position + icit.second,
				position + icit.second + Point<int16_t>(32, 32)
				);
			if (iconrect.contains(cursorpos))
			{
				return slot;
			}
		}
		return 0;
	}

	Optional<Icon> UIEquipInventory::geticon(int16_t slot) const
	{
		return icons.count(slot) ? icons.at(slot).get() : Optional<Icon>();
	}


	UIEquipInventory::EquipIcon::EquipIcon(int16_t s)
	{
		source = s;
	}

	void UIEquipInventory::EquipIcon::drop_on_stage() const
	{
		UnequipItemPacket(source, 0).dispatch();
	}

	void UIEquipInventory::EquipIcon::drop_on_items(Inventory::Type tab, Equipslot::Value eqslot, int16_t slot, bool equip) const
	{
		if (tab != Inventory::EQUIP)
			return;

		if (equip)
		{
			if (eqslot == source)
			{
				EquipItemPacket(slot, eqslot)
					.dispatch();
			}
		}
		else
		{
			UnequipItemPacket(source, slot)
				.dispatch();
		}
	}
}