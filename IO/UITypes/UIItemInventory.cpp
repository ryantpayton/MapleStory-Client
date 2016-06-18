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
#include "UIItemInventory.h"

#include "..\UI.h"
#include "..\Components\MapleButton.h"
#include "..\Components\TwoSpriteButton.h"

#include "..\..\Configuration.h"
#include "..\..\Gameplay\Stage.h"
#include "..\..\Net\Packets\InventoryPackets.h"
#include "..\..\Util\Misc.h"

#include "nlnx\nx.hpp"

namespace jrc
{
	UIItemInventory::UIItemInventory() : 
		UIDragElement<PosINV>(Point<int16_t>(172, 20)), 
		inventory(Stage::get().get_player().get_inventory()) {

		nl::node src = nl::nx::ui["UIWindow2.img"]["Item"];

		sprites.emplace_back(src["backgrnd"]);
		sprites.emplace_back(src["backgrnd2"]);
		sprites.emplace_back(src["backgrnd3"]);

		newitemslot = src["New"]["inventory"];
		newitemtab = src["New"]["Tab0"];
		projectile = src["activeIcon"];

		nl::node taben = src["Tab"]["enabled"];
		nl::node tabdis = src["Tab"]["disabled"];

		buttons[BT_TAB_EQUIP]= std::make_unique<TwoSpriteButton>(tabdis["0"], taben["0"], Point<int16_t>(-1, -4));
		buttons[BT_TAB_USE] = std::make_unique<TwoSpriteButton>(tabdis["1"], taben["1"], Point<int16_t>(-1, -4));
		buttons[BT_TAB_ETC] = std::make_unique<TwoSpriteButton>(tabdis["2"], taben["2"], Point<int16_t>(0, -4));
		buttons[BT_TAB_SETUP] = std::make_unique<TwoSpriteButton>(tabdis["3"], taben["3"], Point<int16_t>(-1, -4));
		buttons[BT_TAB_CASH]= std::make_unique<TwoSpriteButton>(tabdis["4"], taben["4"], Point<int16_t>(-1, -4));

		buttons[BT_DROPMESO]= std::make_unique<MapleButton>(src["BtCoin"]);
		buttons[BT_POINTS]= std::make_unique<MapleButton>(src["BtPoint0"]);
		buttons[BT_GATHER]= std::make_unique<MapleButton>(src["BtGather"]);
		buttons[BT_SORT]= std::make_unique<MapleButton>(src["BtSort"]);
		buttons[BT_EXPAND]= std::make_unique<MapleButton>(src["BtFull"]);
		buttons[BT_ITEMPOT]= std::make_unique<MapleButton>(src["BtPot3"]);
		buttons[BT_UPGRADE]= std::make_unique<MapleButton>(src["BtUpgrade3"]);
		buttons[BT_MAGNIFY]= std::make_unique<MapleButton>(src["BtAppraise3"]);
		buttons[BT_BITCASE]= std::make_unique<MapleButton>(src["BtBits3"]);

		tab = Inventory::EQUIP;
		slotrange.first = 1;
		slotrange.second = 24;
		newtab = Inventory::NONE;
		newslot = 0;

		buttons[BT_SORT]->set_active(false);
		buttons[button_by_tab(tab)]->set_state(Button::PRESSED);

		mesolabel = { Text::A11M, Text::RIGHT, Text::LIGHTGREY };

		slider = {
			11, { 50, 248 }, 152, 6, 1 + inventory.get_slots(tab) / 4, 
			[&](bool upwards) {
			int16_t shift = upwards ? -4 : 4;
			bool above = slotrange.first + shift > 0;
			bool below = slotrange.second + shift < inventory.get_slots(tab) + 1 + 4;
			if (above && below)
			{
				slotrange.first += shift;
				slotrange.second += shift;
			}
		} };

		dimension = { 172, 335 };
		active = true;

		load_icons();
	}

	void UIItemInventory::draw(float alpha) const
	{
		UIElement::draw(alpha);

		slider.draw(position);

		for (auto& icon : icons)
		{
			int16_t slot = icon.first;
			if (icon.second && slot >= slotrange.first && slot <= slotrange.second)
			{
				Point<int16_t> slotpos = get_slotpos(slot);
				icon.second->draw(position + slotpos);
			}
		}

		int16_t bulletslot = inventory.get_bulletslot();
		if (tab == Inventory::USE && is_visible(bulletslot))
		{
			Point<int16_t> bulletslotpos = position + get_slotpos(bulletslot);
			projectile.draw({ bulletslotpos });
		}
		else if (newtab == tab && is_visible(newslot))
		{
			Point<int16_t> newslotpos = position + get_slotpos(newslot);
			newslotpos.shift_y(1);
			newitemslot.draw({ newslotpos }, alpha);
		}
		
		if (newtab != tab && newtab != Inventory::NONE)
		{
			Point<int16_t> newtabpos = position + get_tabpos(newtab);
			newitemtab.draw({ newtabpos }, alpha);
		}

		Point<int16_t> mesopos = position + Point<int16_t>(124, 264);
		mesolabel.draw(mesopos);
	}

	void UIItemInventory::update()
	{
		UIElement::update();

		newitemtab.update(6);
		newitemslot.update(6);

		int64_t meso = Stage::get().get_player().get_inventory().get_meso();
		std::string mesostr = std::to_string(meso);
		string_format::split_number(mesostr);
		mesolabel.change_text(mesostr);
	}

	void UIItemInventory::update_slot(int16_t slot)
	{
		Optional<Item> item = inventory.get_item(tab, slot);
		if (item)
		{
			Optional<const Texture> texture = item
				.transform(&Item::getidata)
				.transform(&ItemData::geticon, false);

			if (texture)
			{
				int32_t itemid = item.map(&Item::get_id);
				int16_t count = item.map(&Item::getcount);
				if (tab == Inventory::EQUIP)
					count = -1;

				Equipslot::Value eqslot = inventory.find_equipslot(itemid);
				icons[slot] = std::make_unique<Icon>(std::make_unique<ItemIcon>(tab, eqslot, slot), *texture, count);
			}
		}
		else if (icons.count(slot))
		{
			icons.erase(slot);
		}
	}

	void UIItemInventory::load_icons()
	{
		icons.clear();

		uint8_t numslots = inventory.get_slots(tab);
		for (uint8_t i = 1; i < numslots; i++)
		{
			update_slot(i);
		}
	}

	Button::State UIItemInventory::button_pressed(uint16_t buttonid)
	{
		Inventory::Type oldtab = tab;
		switch (buttonid)
		{
		case BT_TAB_EQUIP:
			tab = Inventory::EQUIP;
			break;
		case BT_TAB_USE:
			tab = Inventory::USE;
			break;
		case BT_TAB_SETUP:
			tab = Inventory::SETUP;
			break;
		case BT_TAB_ETC:
			tab = Inventory::ETC;
			break;
		case BT_TAB_CASH:
			tab = Inventory::CASH;
			break;
		case BT_GATHER:
			GatherItemsPacket(tab).dispatch();
			break;
		case BT_SORT:
			SortItemsPacket(tab).dispatch();
			break;
		}

		if (tab != oldtab)
		{
			slotrange.first = 1;
			slotrange.second = 24;

			slider.setrows(6, 1 + inventory.get_slots(tab) / 4);

			buttons[button_by_tab(oldtab)]->set_state(Button::NORMAL);

			load_icons();
			enable_gather();
		}
		return Button::PRESSED;
	}

	void UIItemInventory::doubleclick(Point<int16_t> cursorpos)
	{
		int16_t slot = slot_by_position(cursorpos - position);
		if (icons.count(slot) && is_visible(slot))
		{
			Optional<Item> item = inventory.get_item(tab, slot);
			if (item)
			{
				int32_t itemid = item.map(&Item::get_id);
				switch (tab)
				{
				case Inventory::EQUIP:
					EquipItemPacket(slot, inventory.find_equipslot(itemid)).dispatch();
					break;
				case Inventory::USE:
					UseItemPacket(slot, itemid).dispatch();
					break;
				}
			}
		}
	}

	void UIItemInventory::send_icon(const Icon& icon, Point<int16_t> cursorpos)
	{
		int16_t slot = slot_by_position(cursorpos - position);
		if (slot > 0)
		{
			Optional<Item> item = inventory.get_item(tab, slot);
			Equipslot::Value eqslot;
			bool equip;
			if (item && tab == Inventory::EQUIP)
			{
				eqslot = inventory.find_equipslot(item.map(&Item::get_id));
				equip = true;
			}
			else
			{
				eqslot = Equipslot::NONE;
				equip = false;
			}
			icon.drop_on_items(tab, eqslot, slot, equip);
		}
	}

	Cursor::State UIItemInventory::send_cursor(bool pressed, Point<int16_t> cursorpos)
	{
		Cursor::State dstate = UIDragElement::send_cursor(pressed, cursorpos);
		if (dragged)
		{
			clear_tooltip();
			return dstate;
		}

		Point<int16_t> cursor_relative = cursorpos - position;
		if (slider.isenabled())
		{
			Cursor::State sstate = slider.send_cursor(cursor_relative, pressed);
			if (sstate != Cursor::IDLE)
			{
				clear_tooltip();
				return sstate;
			}
		}

		int16_t slot = slot_by_position(cursor_relative);
		Optional<Icon> icon = geticon(slot);
		if (icon && is_visible(slot))
		{
			if (pressed)
			{
				Point<int16_t> slotpos = get_slotpos(slot);
				icon->start_drag(cursor_relative - slotpos);
				UI::get().drag_icon(icon.get());

				clear_tooltip();
				return Cursor::GRABBING;
			}
			else
			{
				show_item(slot);
				return Cursor::CANGRAB;
			}
		}
		else
		{
			clear_tooltip();
			return Cursor::IDLE;
		}
	}

	void UIItemInventory::modify(Inventory::Type type, int16_t slot, int8_t mode, int16_t arg)
	{
		if (slot <= 0)
			return;

		if (type == tab)
		{
			switch (mode)
			{
			case Inventory::ADD:
				update_slot(slot);
				newtab = type;
				newslot = slot;
				break;
			case Inventory::CHANGECOUNT:
			case Inventory::ADDCOUNT:
				geticon(slot)
					.if_present(&Icon::setcount, arg);
				break;
			case Inventory::SWAP:
				if (arg != slot)
				{
					update_slot(slot);
					update_slot(arg);
				}
				break;
			case Inventory::REMOVE:
				update_slot(slot);
				break;
			}
		}

		switch (mode)
		{
		case Inventory::ADD:
		case Inventory::ADDCOUNT:
			newtab = type;
			newslot = slot;
			break;
		case Inventory::CHANGECOUNT:
		case Inventory::SWAP:
		case Inventory::REMOVE:
			if (newslot == slot && newtab == type)
			{
				newslot = 0;
				newtab = Inventory::NONE;
			}
			break;
		}
	}

	void UIItemInventory::enable_sort()
	{
		buttons[BT_GATHER]->set_active(false);
		buttons[BT_SORT]->set_active(true);
		buttons[BT_SORT]->set_state(Button::NORMAL);
	}

	void UIItemInventory::enable_gather()
	{
		buttons[BT_SORT]->set_active(false);
		buttons[BT_GATHER]->set_active(true);
		buttons[BT_GATHER]->set_state(Button::NORMAL);
	}

	void UIItemInventory::toggle_active()
	{
		clear_tooltip();
		UIElement::toggle_active();
	}

	bool UIItemInventory::remove_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (UIDragElement::remove_cursor(clicked, cursorpos))
			return true;

		return slider.remove_cursor(clicked);
	}

	void UIItemInventory::show_item(int16_t slot)
	{
		if (tab == Inventory::EQUIP)
		{
			Optional<Equip> equip = inventory.get_equip(tab, slot);
			UI::get().show_equip(TYPE, equip.get(), slot);
		}
		else
		{
			Optional<Item> item = inventory.get_item(tab, slot);
			int32_t itemid = item.mapordefault(&Item::get_id, 0);
			UI::get().show_item(TYPE, itemid);
		}
	}

	void UIItemInventory::clear_tooltip()
	{
		UI::get().clear_tooltip(TYPE);
	}

	bool UIItemInventory::is_visible(int16_t slot) const
	{
		return !is_not_visible(slot);
	}

	bool UIItemInventory::is_not_visible(int16_t slot) const
	{
		return slot < slotrange.first || slot > slotrange.second;
	}

	int16_t UIItemInventory::slot_by_position(Point<int16_t> cursorpos) const
	{
		int16_t xoff = cursorpos.x() - 11;
		int16_t yoff = cursorpos.y() - 51;
		if (xoff < 1 || xoff > 143 || yoff < 1)
			return 0;

		int16_t slot = slotrange.first + (xoff / 36) + 4 * (yoff / 35);
		return is_visible(slot) ? slot : 0;
	}

	Point<int16_t> UIItemInventory::get_slotpos(int16_t slot) const
	{
		int16_t absslot = slot - slotrange.first;
		return Point<int16_t>(
			11 + (absslot % 4) * 36,
			51 + (absslot / 4) * 35
			);
	}

	Point<int16_t> UIItemInventory::get_tabpos(Inventory::Type tb) const
	{
		switch (tb)
		{
		case Inventory::EQUIP:
			return Point<int16_t>(10, 28);
		case Inventory::USE:
			return Point<int16_t>(42, 28);
		case Inventory::SETUP:
			return Point<int16_t>(74, 28);
		case Inventory::ETC:
			return Point<int16_t>(105, 28);
		case Inventory::CASH:
			return Point<int16_t>(138, 28);
		default:
			return Point<int16_t>();
		}
	}

	uint16_t UIItemInventory::button_by_tab(Inventory::Type tb) const
	{
		switch (tb)
		{
		case Inventory::EQUIP:
			return BT_TAB_EQUIP;
		case Inventory::USE:
			return BT_TAB_USE;
		case Inventory::SETUP:
			return BT_TAB_SETUP;
		case Inventory::ETC:
			return BT_TAB_ETC;
		default:
			return BT_TAB_CASH;
		}
	}

	Optional<Icon> UIItemInventory::geticon(int16_t slot) const
	{
		return icons.count(slot) ? icons.at(slot).get() : Optional<Icon>();
	}


	UIItemInventory::ItemIcon::ItemIcon(Inventory::Type st, Equipslot::Value eqs, int16_t s)
	{
		sourcetab = st;
		eqsource = eqs;
		source = s;
	}

	void UIItemInventory::ItemIcon::drop_on_stage() const
	{
		MoveItemPacket(sourcetab, source, 0, 1).dispatch();
	}

	void UIItemInventory::ItemIcon::drop_on_equips(Equipslot::Value eqslot) const
	{
		switch (sourcetab)
		{
		case Inventory::EQUIP:
			if (eqsource == eqslot)
			{
				EquipItemPacket(source, eqslot).dispatch();
			}
			break;
		case Inventory::USE:
			ScrollEquipPacket(source, eqslot).dispatch();
			break;
		}
	}

	void UIItemInventory::ItemIcon::drop_on_items(Inventory::Type tab, Equipslot::Value, int16_t slot, bool) const
	{
		if (tab != sourcetab || slot == source)
			return;

		MoveItemPacket(tab, source, slot, 1).dispatch();
	}
}