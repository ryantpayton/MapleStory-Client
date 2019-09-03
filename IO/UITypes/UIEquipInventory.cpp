//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "UIEquipInventory.h"

#include "../UI.h"

#include "../Components/MapleButton.h"
#include "../Data/ItemData.h"
#include "../Audio/Audio.h"

#include "../Net/Packets/InventoryPackets.h"
#include "../IO/UITypes/UIItemInventory.h"

#include <nlnx/nx.hpp>

namespace ms
{
	UIEquipInventory::UIEquipInventory(const Inventory& invent) : UIDragElement<PosEQINV>(Point<int16_t>(0, 0)), inventory(invent), showpetequips(false)
	{
		iconpositions[Equipslot::Id::CAP] = Point<int16_t>(43, 25);
		iconpositions[Equipslot::Id::FACEACC] = Point<int16_t>(43, 91);
		iconpositions[Equipslot::Id::EYEACC] = Point<int16_t>(43, 68);
		iconpositions[Equipslot::Id::EARRINGS] = Point<int16_t>(109, 91);
		iconpositions[Equipslot::Id::TOP] = Point<int16_t>(43, 124);
		iconpositions[Equipslot::Id::PANTS] = Point<int16_t>(43, 157);
		iconpositions[Equipslot::Id::SHOES] = Point<int16_t>(76, 190);
		iconpositions[Equipslot::Id::GLOVES] = Point<int16_t>(10, 157);
		iconpositions[Equipslot::Id::CAPE] = Point<int16_t>(142, 124);
		iconpositions[Equipslot::Id::SHIELD] = Point<int16_t>(142, 124);
		iconpositions[Equipslot::Id::WEAPON] = Point<int16_t>(109, 124);
		iconpositions[Equipslot::Id::RING] = Point<int16_t>(109, 157);
		iconpositions[Equipslot::Id::RING2] = Point<int16_t>(142, 157);
		iconpositions[Equipslot::Id::RING3] = Point<int16_t>(109, 91);
		iconpositions[Equipslot::Id::RING4] = Point<int16_t>(142, 91);
		iconpositions[Equipslot::Id::PENDANT] = Point<int16_t>(76, 124);
		iconpositions[Equipslot::Id::TAMEDMOB] = Point<int16_t>(142, 91);
		iconpositions[Equipslot::Id::SADDLE] = Point<int16_t>(76, 124);
		iconpositions[Equipslot::Id::MEDAL] = Point<int16_t>(10, 58);
		iconpositions[Equipslot::Id::BELT] = Point<int16_t>(76, 157);

		nl::node close = nl::nx::ui["Basic.img"]["BtClose"];
		nl::node source = nl::nx::ui["UIWindow2.img"]["Equip"]["character"];
		nl::node petsource = nl::nx::ui["UIWindow2.img"]["Equip"]["pet"];
		nl::node backgrnd = source["backgrnd"];

		Point<int16_t> bg_dimensions = Texture(backgrnd).get_dimensions();

		sprites.emplace_back(backgrnd);
		sprites.emplace_back(source["backgrnd2"]);
		sprites.emplace_back(source["backgrnd3_Kanna"]);
		sprites.emplace_back(source["cashPendant"]);
		sprites.emplace_back(source["charmPocket"]);
		sprites.emplace_back(source["emblem"]);

		buttons[Buttons::BT_CLOSE] = std::make_unique<MapleButton>(close, Point<int16_t>(bg_dimensions.x() - 17, 12));
		buttons[Buttons::BT_TOGGLEPETS] = std::make_unique<MapleButton>(source["BtPet"]);

		textures_pet.emplace_back(petsource["backgrnd"]);
		textures_pet.emplace_back(petsource["backgrnd2"]);
		textures_pet.emplace_back(petsource["backgrnd3"]);

		dimension = bg_dimensions;
		dragarea = Point<int16_t>(bg_dimensions.x(), 20);

		load_icons();
	}

	void UIEquipInventory::draw(float alpha) const
	{
		UIElement::draw(alpha);

		for (auto iter : icons)
			if (iter.second)
				iter.second->draw(position + iconpositions[iter.first]);

		if (showpetequips)
		{
			Point<int16_t> position_pet(position + Point<int16_t>(184, 0));

			for (auto& texture : textures_pet)
				texture.draw(position_pet);
		}
	}

	Button::State UIEquipInventory::button_pressed(uint16_t id)
	{
		switch (id)
		{
		case Buttons::BT_CLOSE:
			toggle_active();
			return Button::State::NORMAL;
		case Buttons::BT_TOGGLEPETS:
			showpetequips = !showpetequips;

			return Button::State::NORMAL;
		default:
			return Button::State::PRESSED;
		}
	}

	void UIEquipInventory::update_slot(Equipslot::Id slot)
	{
		if (int32_t item_id = inventory.get_item_id(InventoryType::Id::EQUIPPED, slot))
		{
			const Texture& texture = ItemData::get(item_id).get_icon(false);

			icons[slot] = std::make_unique<Icon>(
				std::make_unique<EquipIcon>(slot),
				texture,
				-1
				);
		}
		else if (icons[slot])
		{
			icons[slot].release();
		}

		clear_tooltip();
	}

	void UIEquipInventory::load_icons()
	{
		icons.clear();

		for (auto iter : Equipslot::values)
			update_slot(iter);
	}

	Cursor::State UIEquipInventory::send_cursor(bool pressed, Point<int16_t> cursorpos)
	{
		Cursor::State dstate = UIDragElement::send_cursor(pressed, cursorpos);

		if (dragged)
		{
			clear_tooltip();

			return dstate;
		}

		Equipslot::Id slot = slot_by_position(cursorpos);

		if (auto icon = icons[slot].get())
		{
			if (pressed)
			{
				icon->start_drag(cursorpos - position - iconpositions[slot]);

				UI::get().drag_icon(icon);

				clear_tooltip();

				return Cursor::State::GRABBING;
			}
			else
			{
				show_equip(slot);

				return Cursor::State::CANGRAB;
			}
		}
		else
		{
			clear_tooltip();

			return Cursor::State::IDLE;
		}
	}

	void UIEquipInventory::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed && escape)
		{
			clear_tooltip();
			deactivate();
		}
	}

	void UIEquipInventory::doubleclick(Point<int16_t> cursorpos)
	{
		Equipslot::Id slot = slot_by_position(cursorpos);

		if (icons[slot])
			if (int16_t freeslot = inventory.find_free_slot(InventoryType::Id::EQUIP))
				UnequipItemPacket(slot, freeslot).dispatch();
	}

	bool UIEquipInventory::send_icon(const Icon& icon, Point<int16_t> cursorpos)
	{
		if (Equipslot::Id slot = slot_by_position(cursorpos))
			icon.drop_on_equips(slot);

		return true;
	}

	void UIEquipInventory::toggle_active()
	{
		clear_tooltip();

		UIElement::toggle_active();
	}

	void UIEquipInventory::modify(int16_t pos, int8_t mode, int16_t arg)
	{
		Equipslot::Id eqpos = Equipslot::by_id(pos);
		Equipslot::Id eqarg = Equipslot::by_id(arg);

		switch (mode)
		{
		case 0:
		case 3:
			update_slot(eqpos);
			break;
		case 2:
			update_slot(eqpos);
			update_slot(eqarg);
			break;
		}
	}

	void UIEquipInventory::show_equip(Equipslot::Id slot)
	{
		UI::get().show_equip(Tooltip::Parent::EQUIPINVENTORY, slot);
	}

	void UIEquipInventory::clear_tooltip()
	{
		UI::get().clear_tooltip(Tooltip::Parent::EQUIPINVENTORY);
	}

	Equipslot::Id UIEquipInventory::slot_by_position(Point<int16_t> cursorpos) const
	{
		for (auto iter : iconpositions)
		{
			Rectangle<int16_t> iconrect = Rectangle<int16_t>(
				position + iter.second,
				position + iter.second + Point<int16_t>(32, 32)
				);

			if (iconrect.contains(cursorpos))
				return iter.first;
		}

		return Equipslot::Id::NONE;
	}

	UIEquipInventory::EquipIcon::EquipIcon(int16_t s)
	{
		source = s;
	}

	void UIEquipInventory::EquipIcon::drop_on_stage() const
	{
		Sound(Sound::Name::DRAGEND).play();
	}

	void UIEquipInventory::EquipIcon::drop_on_equips(Equipslot::Id slot) const
	{
		if (source == slot)
			Sound(Sound::Name::DRAGEND).play();
	}

	bool UIEquipInventory::EquipIcon::drop_on_items(InventoryType::Id tab, Equipslot::Id eqslot, int16_t slot, bool equip) const
	{
		if (tab != InventoryType::Id::EQUIP)
		{
			if (auto iteminventory = UI::get().get_element<UIItemInventory>())
			{
				if (iteminventory->is_active())
				{
					iteminventory->change_tab(InventoryType::Id::EQUIP);
					return false;
				}
			}
		}

		if (equip)
		{
			if (eqslot == source)
				EquipItemPacket(slot, eqslot).dispatch();
		}
		else
		{
			UnequipItemPacket(source, slot).dispatch();
		}

		return true;
	}
}