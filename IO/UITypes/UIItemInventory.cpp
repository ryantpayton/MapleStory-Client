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
#include "UIItemInventory.h"

#include "UINotice.h"

#include "../UI.h"

#include "../Components/MapleButton.h"
#include "../UITypes/UIKeyConfig.h"

#include "../../Data/EquipData.h"
#include "../../Gameplay/Stage.h"

#include "../../Net/Packets/InventoryPackets.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UIItemInventory::UIItemInventory(const Inventory& invent) : UIDragElement<PosINV>(), inventory(invent), ignore_tooltip(false), tab(InventoryType::Id::EQUIP), sort_enabled(true)
	{
		nl::node Item = nl::nx::UI["UIWindow2.img"]["Item"];
		nl::node pos = Item["pos"];

		slot_col = pos["slot_col"];
		slot_pos = pos["slot_pos"];
		slot_row = pos["slot_row"];
		slot_space_x = pos["slot_space_x"];
		slot_space_y = pos["slot_space_y"];

		max_slots = slot_row * slot_col;
		max_full_slots = slot_col * max_slots;

		nl::node AutoBuild = Item["AutoBuild"];
		nl::node FullAutoBuild = Item["FullAutoBuild"];

		backgrnd = Item["productionBackgrnd"];
		backgrnd2 = Item["productionBackgrnd2"];
		backgrnd3 = Item["productionBackgrnd3"];

		full_backgrnd = Item["FullBackgrnd"];
		full_backgrnd2 = Item["FullBackgrnd2"];
		full_backgrnd3 = Item["FullBackgrnd3"];

		bg_dimensions = backgrnd.get_dimensions();
		bg_full_dimensions = full_backgrnd.get_dimensions();

		nl::node New = Item["New"];
		newitemslot = New["inventory"];
		newitemtabdis = New["Tab0"];
		newitemtaben = New["Tab1"];

		projectile = Item["activeIcon"];
		disabled = Item["disabled"];

		Point<int16_t> icon_dimensions = disabled.get_dimensions();
		icon_width = icon_dimensions.x();
		icon_height = icon_dimensions.y();

		nl::node Tab = Item["Tab"];
		nl::node taben = Tab["enabled"];
		nl::node tabdis = Tab["disabled"];

		Point<int16_t> tab_pos0 = Texture(taben["0"]).get_origin() * -1;
		Point<int16_t> tab_pos1 = Texture(taben["1"]).get_origin() * -1;
		Point<int16_t> tab_pos2 = Texture(taben["2"]).get_origin() * -1;
		Point<int16_t> tab_pos3 = Texture(taben["3"]).get_origin() * -1;
		Point<int16_t> tab_pos4 = Texture(taben["4"]).get_origin() * -1;
		Point<int16_t> tab_pos5 = Texture(taben["5"]).get_origin() * -1;
		Point<int16_t> tab_pos_adj = Point<int16_t>(9, 26);

		nl::node close = nl::nx::UI["Basic.img"]["BtClose3"];
		buttons[Buttons::BT_CLOSE] = std::make_unique<MapleButton>(close);

		buttons[Buttons::BT_TAB_EQUIP] = std::make_unique<TwoSpriteButton>(tabdis["0"], taben["0"], tab_pos0 - tab_pos_adj, Point<int16_t>(0, 0));
		buttons[Buttons::BT_TAB_USE] = std::make_unique<TwoSpriteButton>(tabdis["1"], taben["1"], tab_pos1 - tab_pos_adj, Point<int16_t>(0, 0));
		buttons[Buttons::BT_TAB_ETC] = std::make_unique<TwoSpriteButton>(tabdis["2"], taben["2"], tab_pos2 - tab_pos_adj, Point<int16_t>(0, 0));
		buttons[Buttons::BT_TAB_SETUP] = std::make_unique<TwoSpriteButton>(tabdis["3"], taben["3"], tab_pos3 - tab_pos_adj, Point<int16_t>(0, 0));
		buttons[Buttons::BT_TAB_CASH] = std::make_unique<TwoSpriteButton>(tabdis["4"], taben["4"], tab_pos4 - tab_pos_adj, Point<int16_t>(0, 0));
		buttons[Buttons::BT_TAB_DEC] = std::make_unique<TwoSpriteButton>(tabdis["5"], taben["5"], tab_pos5 - tab_pos_adj, Point<int16_t>(0, 0));

		buttons[Buttons::BT_COIN] = std::make_unique<MapleButton>(AutoBuild["button:Coin"]);
		buttons[Buttons::BT_POINT] = std::make_unique<MapleButton>(AutoBuild["button:Point"]);
		buttons[Buttons::BT_GATHER] = std::make_unique<MapleButton>(AutoBuild["button:Gather"]);
		buttons[Buttons::BT_SORT] = std::make_unique<MapleButton>(AutoBuild["button:Sort"]);
		buttons[Buttons::BT_FULL] = std::make_unique<MapleButton>(AutoBuild["button:Full"]);
		buttons[Buttons::BT_UPGRADE] = std::make_unique<MapleButton>(AutoBuild["button:Upgrade"]);
		buttons[Buttons::BT_APPRAISE] = std::make_unique<MapleButton>(AutoBuild["button:Appraise"]);
		buttons[Buttons::BT_EXTRACT] = std::make_unique<MapleButton>(AutoBuild["button:Extract"]);
		buttons[Buttons::BT_DISASSEMBLE] = std::make_unique<MapleButton>(AutoBuild["button:Disassemble"]);
		buttons[Buttons::BT_TOAD] = std::make_unique<MapleButton>(AutoBuild["anibutton:Toad"]);

		buttons[Buttons::BT_SMALL] = std::make_unique<MapleButton>(FullAutoBuild["button:Small"]);
		buttons[Buttons::BT_COIN_SM] = std::make_unique<MapleButton>(FullAutoBuild["button:Coin"]);
		buttons[Buttons::BT_POINT_SM] = std::make_unique<MapleButton>(FullAutoBuild["button:Point"]);
		buttons[Buttons::BT_GATHER_SM] = std::make_unique<MapleButton>(FullAutoBuild["button:Gather"]);
		buttons[Buttons::BT_SORT_SM] = std::make_unique<MapleButton>(FullAutoBuild["button:Sort"]);
		buttons[Buttons::BT_UPGRADE_SM] = std::make_unique<MapleButton>(FullAutoBuild["button:Upgrade"]);
		buttons[Buttons::BT_APPRAISE_SM] = std::make_unique<MapleButton>(FullAutoBuild["button:Appraise"]);
		buttons[Buttons::BT_EXTRACT_SM] = std::make_unique<MapleButton>(FullAutoBuild["button:Extract"]);
		buttons[Buttons::BT_DISASSEMBLE_SM] = std::make_unique<MapleButton>(FullAutoBuild["button:Disassemble"]);
		buttons[Buttons::BT_TOAD_SM] = std::make_unique<MapleButton>(FullAutoBuild["anibutton:Toad"]);
		buttons[Buttons::BT_CASHSHOP] = std::make_unique<MapleButton>(FullAutoBuild["button:Cashshop"]);

		buttons[Buttons::BT_EXTRACT]->set_state(Button::State::DISABLED);
		buttons[Buttons::BT_EXTRACT_SM]->set_state(Button::State::DISABLED);
		buttons[Buttons::BT_DISASSEMBLE]->set_state(Button::State::DISABLED);
		buttons[Buttons::BT_DISASSEMBLE_SM]->set_state(Button::State::DISABLED);
		buttons[button_by_tab(tab)]->set_state(Button::State::PRESSED);

		mesolabel = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::BLACK);
		maplepointslabel = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::BLACK);
		maplepointslabel.change_text("0"); // TODO: Implement

		for (size_t i = 0; i < InventoryType::Id::LENGTH; i++)
		{
			InventoryType::Id id = InventoryType::by_value(i);
			slotrange[id] = std::pair<int16_t, int16_t>(1, 24);
		}

		int16_t second = (icon_height + slot_space_y) * inventory.get_slotmax(tab) / slot_col + 24;
		int16_t x = slot_col * (icon_width + slot_space_x) + 4;
		int16_t unitrows = slot_row - 2;
		int16_t rowmax = inventory.get_slotmax(tab) / slot_col;

		slider = Slider(
			Slider::Type::DEFAULT_SILVER, Range<int16_t>(slot_pos.y(), second), x, unitrows, rowmax,
			[&](bool upwards)
			{
				int16_t shift = upwards ? -slot_col : slot_col;

				slotrange[tab].first += shift;
				slotrange[tab].second += shift;
			}
		);

#if LOG_LEVEL >= LOG_UI
		for (size_t i = 0; i < max_full_slots; i++)
			slot_labels[i] = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::BLACK, std::to_string(i + 1));
#endif

		set_full(false);
		clear_new();
		load_icons();
	}

	void UIItemInventory::draw(float alpha) const
	{
		UIElement::draw_sprites(alpha);

		Point<int16_t> mesolabel_pos = position + Point<int16_t>(144, 305);
		Point<int16_t> maplepointslabel_pos = position + Point<int16_t>(179, 323);

		if (full_enabled)
		{
			full_backgrnd.draw(position);
			full_backgrnd2.draw(position);
			full_backgrnd3.draw(position);

			mesolabel.draw(mesolabel_pos + Point<int16_t>(0, 84));
			maplepointslabel.draw(maplepointslabel_pos + Point<int16_t>(220, 66));
		}
		else
		{
			backgrnd.draw(position);
			backgrnd2.draw(position);
			backgrnd3.draw(position);

			slider.draw(position + Point<int16_t>(0, 1));

			mesolabel.draw(mesolabel_pos);
			maplepointslabel.draw(maplepointslabel_pos);
		}

		auto& range = slotrange.at(tab);

		size_t numslots = inventory.get_slotmax(tab);
		size_t firstslot = full_enabled ? 1 : range.first;
		size_t lastslot = full_enabled ? max_full_slots : range.second;

		for (size_t i = 0; i <= max_full_slots; i++)
		{
			Point<int16_t> slotpos = get_slotpos(i);

			if (icons.find(i) != icons.end())
			{
				auto& icon = icons.at(i);

				if (icon && i >= firstslot && i <= lastslot)
					icon->draw(position + slotpos);
			}
			else
			{
				if (i > numslots && i <= lastslot)
					disabled.draw(position + slotpos);
			}

#if LOG_LEVEL >= LOG_UI
			if (i <= lastslot && i < max_full_slots)
				slot_labels[i].draw(position + get_slotpos(i + 1) - Point<int16_t>(0, 5));
#endif
		}

		int16_t bulletslot = inventory.get_bulletslot();

		if (tab == InventoryType::Id::USE && is_visible(bulletslot))
			projectile.draw(position + get_slotpos(bulletslot));

		UIElement::draw_buttons(alpha);

		if (newtab != InventoryType::Id::NONE)
		{
			if (newtab == tab)
			{
				newitemtaben.draw(position + get_tabpos(newtab) - Point<int16_t>(2, 3), alpha);

				if (is_visible(newslot))
					newitemslot.draw(position + get_slotpos(newslot) + Point<int16_t>(1, 1), alpha);
			}
			else
			{
				newitemtabdis.draw(position + get_tabpos(newtab) - Point<int16_t>(2, 1), alpha);
			}
		}
	}

	void UIItemInventory::update()
	{
		UIElement::update();

		newitemtaben.update(6);
		newitemtabdis.update(6);
		newitemslot.update(6);

		std::string meso_str = std::to_string(inventory.get_meso());
		string_format::split_number(meso_str);

		mesolabel.change_text(meso_str);
	}

	void UIItemInventory::update_slot(int16_t slot)
	{
		if (int32_t item_id = inventory.get_item_id(tab, slot))
		{
			int16_t count;

			if (tab == InventoryType::Id::EQUIP)
				count = -1;
			else
				count = inventory.get_item_count(tab, slot);

			const bool untradable = ItemData::get(item_id).is_untradable();
			const bool cashitem = ItemData::get(item_id).is_cashitem();
			const Texture& texture = ItemData::get(item_id).get_icon(false);
			EquipSlot::Id eqslot = inventory.find_equipslot(item_id);

			icons[slot] = std::make_unique<Icon>(
				std::make_unique<ItemIcon>(*this, tab, eqslot, slot, item_id, count, untradable, cashitem),
				texture, count
			);
		}
		else if (icons.count(slot))
		{
			icons.erase(slot);
		}
	}

	void UIItemInventory::load_icons()
	{
		icons.clear();

		uint8_t numslots = inventory.get_slotmax(tab);

		for (size_t i = 0; i <= max_full_slots; i++)
			if (i <= numslots)
				update_slot(i);
	}

	Button::State UIItemInventory::button_pressed(uint16_t buttonid)
	{
		InventoryType::Id oldtab = tab;

		switch (buttonid)
		{
			case Buttons::BT_CLOSE:
			{
				toggle_active();

				return Button::State::NORMAL;
			}
			case Buttons::BT_TAB_EQUIP:
			{
				tab = InventoryType::Id::EQUIP;
				break;
			}
			case Buttons::BT_TAB_USE:
			{
				tab = InventoryType::Id::USE;
				break;
			}
			case Buttons::BT_TAB_SETUP:
			{
				tab = InventoryType::Id::SETUP;
				break;
			}
			case Buttons::BT_TAB_ETC:
			{
				tab = InventoryType::Id::ETC;
				break;
			}
			case Buttons::BT_TAB_CASH:
			{
				tab = InventoryType::Id::CASH;
				break;
			}
			case Buttons::BT_TAB_DEC:
			{
				tab = InventoryType::Id::DEC;
				break;
			}
			case Buttons::BT_GATHER:
			case Buttons::BT_GATHER_SM:
			{
				GatherItemsPacket(tab).dispatch();
				break;
			}
			case Buttons::BT_SORT:
			case Buttons::BT_SORT_SM:
			{
				SortItemsPacket(tab).dispatch();
				break;
			}
			case Buttons::BT_FULL:
			{
				set_full(true);

				return Button::State::NORMAL;
			}
			case Buttons::BT_SMALL:
			{
				set_full(false);

				return Button::State::NORMAL;
			}
			case Buttons::BT_COIN:
			case Buttons::BT_COIN_SM:
			case Buttons::BT_POINT:
			case Buttons::BT_POINT_SM:
			case Buttons::BT_UPGRADE:
			case Buttons::BT_UPGRADE_SM:
			case Buttons::BT_APPRAISE:
			case Buttons::BT_APPRAISE_SM:
			case Buttons::BT_EXTRACT:
			case Buttons::BT_EXTRACT_SM:
			case Buttons::BT_DISASSEMBLE:
			case Buttons::BT_DISASSEMBLE_SM:
			case Buttons::BT_TOAD:
			case Buttons::BT_TOAD_SM:
			case Buttons::BT_CASHSHOP:
			{
				return Button::State::NORMAL;
			}
		}

		if (tab != oldtab)
		{
			uint16_t row = slotrange.at(tab).first / slot_col;
			slider.setrows(row, 6, inventory.get_slotmax(tab) / slot_col);

			buttons[button_by_tab(oldtab)]->set_state(Button::State::NORMAL);
			buttons[button_by_tab(tab)]->set_state(Button::State::PRESSED);

			load_icons();
			set_sort(false);
		}

		return Button::State::IDENTITY;
	}

	void UIItemInventory::doubleclick(Point<int16_t> cursorpos)
	{
		int16_t slot = slot_by_position(cursorpos - position);

		if (icons.count(slot) && is_visible(slot))
		{
			if (int32_t item_id = inventory.get_item_id(tab, slot))
			{
				switch (tab)
				{
					case InventoryType::Id::EQUIP:
					{
						if (can_wear_equip(slot))
						{
							EquipSlot::Id equipslot = inventory.find_equipslot(item_id);

							if (equipslot == EquipSlot::Id::NONE)
							{
								LOG(LOG_DEBUG, "Could not find appropriate EquipSlot::Id for item [" << item_id << "]. Equip would be dropped.");
								break;
							}

							EquipItemPacket(slot, equipslot).dispatch();
						}

						break;
					}
					case InventoryType::Id::USE:
					{
						UseItemPacket(slot, item_id).dispatch();
						break;
					}
				}
			}
		}
	}

	bool UIItemInventory::send_icon(const Icon& icon, Point<int16_t> cursorpos)
	{
		int16_t slot = slot_by_position(cursorpos - position);

		if (slot > 0)
		{
			int32_t item_id = inventory.get_item_id(tab, slot);
			EquipSlot::Id eqslot;
			bool equip;

			if (item_id && tab == InventoryType::Id::EQUIP)
			{
				eqslot = inventory.find_equipslot(item_id);
				equip = true;
			}
			else
			{
				eqslot = EquipSlot::Id::NONE;
				equip = false;
			}

			ignore_tooltip = true;

			return icon.drop_on_items(tab, eqslot, slot, equip);
		}

		return true;
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

		if (!full_enabled && slider.isenabled())
		{
			Cursor::State sstate = slider.send_cursor(cursor_relative, pressed);

			if (sstate != Cursor::State::IDLE)
			{
				clear_tooltip();

				return sstate;
			}
		}

		int16_t slot = slot_by_position(cursor_relative);
		Icon* icon = get_icon(slot);
		bool is_icon = icon && is_visible(slot);

		if (is_icon)
		{
			if (pressed)
			{
				Point<int16_t> slotpos = get_slotpos(slot);
				icon->start_drag(cursor_relative - slotpos);

				UI::get().drag_icon(icon);

				clear_tooltip();

				return Cursor::State::GRABBING;
			}
			else if (!ignore_tooltip)
			{
				show_item(slot);

				return Cursor::State::CANGRAB;
			}
			else
			{
				ignore_tooltip = false;

				return Cursor::State::CANGRAB;
			}
		}
		else
		{
			clear_tooltip();

			return UIElement::send_cursor(pressed, cursorpos);
		}
	}

	void UIItemInventory::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
			{
				toggle_active();
			}
			else if (keycode == KeyAction::Id::TAB)
			{
				clear_tooltip();

				InventoryType::Id newtab;

				switch (tab)
				{
					case InventoryType::Id::EQUIP:
						newtab = InventoryType::Id::USE;
						break;
					case InventoryType::Id::USE:
						newtab = InventoryType::Id::ETC;
						break;
					case InventoryType::Id::ETC:
						newtab = InventoryType::Id::SETUP;
						break;
					case InventoryType::Id::SETUP:
						newtab = InventoryType::Id::CASH;
						break;
					case InventoryType::Id::CASH:
						newtab = InventoryType::Id::DEC;
						break;
					case InventoryType::Id::DEC:
						newtab = InventoryType::Id::EQUIP;
						break;
				}

				button_pressed(button_by_tab(newtab));
			}
		}
	}

	UIElement::Type UIItemInventory::get_type() const
	{
		return TYPE;
	}

	void UIItemInventory::modify(InventoryType::Id type, int16_t slot, int8_t mode, int16_t arg)
	{
		if (slot <= 0)
			return;

		if (type == tab)
		{
			switch (mode)
			{
				case Inventory::Modification::ADD:
				{
					update_slot(slot);

					newtab = type;
					newslot = slot;
					break;
				}
				case Inventory::Modification::CHANGECOUNT:
				case Inventory::Modification::ADDCOUNT:
				{
					if (auto icon = get_icon(slot))
						icon->set_count(arg);

					break;
				}
				case Inventory::Modification::SWAP:
				{
					if (arg != slot)
					{
						update_slot(slot);
						update_slot(arg);
					}

					break;
				}
				case Inventory::Modification::REMOVE:
				{
					update_slot(slot);
					break;
				}
			}
		}

		switch (mode)
		{
			case Inventory::Modification::ADD:
			case Inventory::Modification::ADDCOUNT:
			{
				newtab = type;
				newslot = slot;
				break;
			}
			case Inventory::Modification::CHANGECOUNT:
			case Inventory::Modification::SWAP:
			case Inventory::Modification::REMOVE:
			{
				if (newslot == slot && newtab == type)
					clear_new();

				break;
			}
		}
	}

	void UIItemInventory::set_sort(bool enabled)
	{
		sort_enabled = enabled;

		if (full_enabled)
		{
			if (sort_enabled)
			{
				buttons[Buttons::BT_SORT]->set_active(false);
				buttons[Buttons::BT_SORT_SM]->set_active(true);
				buttons[Buttons::BT_GATHER]->set_active(false);
				buttons[Buttons::BT_GATHER_SM]->set_active(false);
			}
			else
			{
				buttons[Buttons::BT_SORT]->set_active(false);
				buttons[Buttons::BT_SORT_SM]->set_active(false);
				buttons[Buttons::BT_GATHER]->set_active(false);
				buttons[Buttons::BT_GATHER_SM]->set_active(true);
			}
		}
		else
		{
			if (sort_enabled)
			{
				buttons[Buttons::BT_SORT]->set_active(true);
				buttons[Buttons::BT_SORT_SM]->set_active(false);
				buttons[Buttons::BT_GATHER]->set_active(false);
				buttons[Buttons::BT_GATHER_SM]->set_active(false);
			}
			else
			{
				buttons[Buttons::BT_SORT]->set_active(false);
				buttons[Buttons::BT_SORT_SM]->set_active(false);
				buttons[Buttons::BT_GATHER]->set_active(true);
				buttons[Buttons::BT_GATHER_SM]->set_active(false);
			}
		}
	}

	void UIItemInventory::change_tab(InventoryType::Id type)
	{
		button_pressed(button_by_tab(type));
	}

	void UIItemInventory::clear_new()
	{
		newtab = InventoryType::Id::NONE;
		newslot = 0;
	}

	void UIItemInventory::toggle_active()
	{
		UIElement::toggle_active();

		if (!active)
		{
			clear_new();
			clear_tooltip();
		}
	}

	void UIItemInventory::remove_cursor()
	{
		UIDragElement::remove_cursor();

		slider.remove_cursor();
	}

	void UIItemInventory::show_item(int16_t slot)
	{
		if (tab == InventoryType::Id::EQUIP)
		{
			UI::get().show_equip(Tooltip::Parent::ITEMINVENTORY, slot);
		}
		else
		{
			int32_t item_id = inventory.get_item_id(tab, slot);
			UI::get().show_item(Tooltip::Parent::ITEMINVENTORY, item_id);
		}
	}

	void UIItemInventory::clear_tooltip()
	{
		UI::get().clear_tooltip(Tooltip::Parent::ITEMINVENTORY);
	}

	bool UIItemInventory::is_visible(int16_t slot) const
	{
		return !is_not_visible(slot);
	}

	bool UIItemInventory::is_not_visible(int16_t slot) const
	{
		auto& range = slotrange.at(tab);

		if (full_enabled)
			return slot < 1 || slot > max_full_slots;
		else
			return slot < range.first || slot > range.second;
	}

	bool UIItemInventory::can_wear_equip(int16_t slot) const
	{
		const Player& player = Stage::get().get_player();
		const CharStats& stats = player.get_stats();
		const CharLook& look = player.get_look();
		const bool alerted = look.get_alerted();

		if (alerted)
		{
			UI::get().emplace<UIOk>("You cannot complete this action right now.\\nEvade the attack and try again.", [](bool) {});
			return false;
		}

		const int32_t item_id = inventory.get_item_id(InventoryType::Id::EQUIP, slot);
		const EquipData& equipdata = EquipData::get(item_id);
		const ItemData& itemdata = equipdata.get_itemdata();

		const int8_t reqGender = itemdata.get_gender();
		const bool female = stats.get_female();

		switch (reqGender)
		{
			// Male
			case 0:
			{
				if (female)
					return false;

				break;
			}
			// Female
			case 1:
			{
				if (!female)
					return false;

				break;
			}
			// Unisex
			case 2:
			default:
			{
				break;
			}
		}

		const std::string jobname = stats.get_jobname();

		if (jobname == "GM" || jobname == "SuperGM")
			return true;

		// TODO: Remove from EquipTooltip and move into Job?
		bool can_wear = false;

		uint16_t job = stats.get_stat(MapleStat::Id::JOB) / 100;
		int16_t reqJOB = equipdata.get_reqstat(MapleStat::Id::JOB);

		switch (reqJOB)
		{
			case 0: // Common
			{
				can_wear = true;
				break;
			}
			case 1: // Warrior
			{
				if (job == 1 || job >= 20)
					can_wear = true;

				break;
			}
			case 2: // Magician
			{
				if (job == 2)
					can_wear = true;

				break;
			}
			case 3: // Magician, Warrior
			{
				if (job == 1 || job >= 20 || job == 2)
					can_wear = true;

				break;
			}
			case 4: // Bowman
			{
				if (job == 3)
					can_wear = true;

				break;
			}
			case 8: // Thief
			{
				if (job == 4)
					can_wear = true;

				break;
			}
			case 16: // Pirate
			{
				if (job == 5)
					can_wear = true;

				break;
			}
			default:
			{
				can_wear = false;
			}
		}

		if (!can_wear)
		{
			UI::get().emplace<UIOk>("Your current job\\ncannot equip the selected item.", [](bool) {});
			return false;
		}
		// End of TODO

		int16_t reqLevel = equipdata.get_reqstat(MapleStat::Id::LEVEL);
		int16_t reqDEX = equipdata.get_reqstat(MapleStat::Id::DEX);
		int16_t reqSTR = equipdata.get_reqstat(MapleStat::Id::STR);
		int16_t reqLUK = equipdata.get_reqstat(MapleStat::Id::LUK);
		int16_t reqINT = equipdata.get_reqstat(MapleStat::Id::INT);
		int16_t reqFAME = equipdata.get_reqstat(MapleStat::Id::FAME);

		int8_t i = 0;

		if (reqLevel > stats.get_stat(MapleStat::Id::LEVEL))
			i++;
		else if (reqDEX > stats.get_total(EquipStat::Id::DEX))
			i++;
		else if (reqSTR > stats.get_total(EquipStat::Id::STR))
			i++;
		else if (reqLUK > stats.get_total(EquipStat::Id::LUK))
			i++;
		else if (reqINT > stats.get_total(EquipStat::Id::INT))
			i++;
		else if (reqFAME > stats.get_honor())
			i++;

		if (i > 0)
		{
			UI::get().emplace<UIOk>("Your stats are too low to equip this item\\nor you do not meet the job requirement.", [](bool) {});
			return false;
		}

		return true;
	}

	int16_t UIItemInventory::slot_by_position(Point<int16_t> cursorpos) const
	{
		Point<int16_t> cursor_offset = cursorpos - slot_pos;

		int16_t xoff = cursor_offset.x();
		int16_t yoff = cursor_offset.y();

		int16_t cur_x = cursorpos.x();
		int16_t slot_x = slot_pos.x();
		int16_t xmin = slot_x;
		int16_t xmax = (icon_width + slot_space_x) * (full_enabled ? slot_col * 4 : slot_col) - (full_enabled ? slot_space_x : 0);

		int16_t cur_y = cursorpos.y();
		int16_t slot_y = slot_pos.y();
		int16_t ymin = slot_y;
		int16_t ymax = (icon_height + slot_space_y) * (full_enabled ? slot_row + 1 : slot_row - 1) - (full_enabled ? slot_space_y : 0);

		int16_t slot = 0;
		int16_t absslot = full_enabled ? 1 : slotrange.at(tab).first;

		int16_t col = cur_x / (icon_width + slot_space_x);
		int16_t row = cur_y / (icon_height + slot_space_y) - 1;

		div_t div = std::div(col, 4);
		slot = col + absslot + (4 * row) + (div.quot * 28);

		if (cur_x < xmin || cur_x > xmax || cur_y < ymin || cur_y > ymax)
			slot = 0;

		LOG(LOG_UI,
			"Slot: " << slot << " Col: " << col << " Row: " << row << " "
			<< cur_x << " < (" << xmin << ") || "
			<< cur_x << " > (" << xmax << ") || "
			<< cur_y << " < (" << ymin << ") && "
			<< cur_y << " > (" << ymax << ")");

		if (is_visible(slot))
			return slot;

		return 0;
	}

	Point<int16_t> UIItemInventory::get_slotpos(int16_t slot) const
	{
		int16_t absslot = slot - (full_enabled ? 1 : slotrange.at(tab).first);

		div_t div4 = std::div(absslot, 4);
		div_t div32 = std::div(absslot, 32);

		int16_t row = div4.quot - (8 * div32.quot);
		int16_t col = div4.rem + (4 * div32.quot);

		return slot_pos + Point<int16_t>((col * 10) + (col * 32), (row * 10) + (row * 32));
	}

	Point<int16_t> UIItemInventory::get_tabpos(InventoryType::Id tb) const
	{
		int8_t fixed_tab = tb;

		switch (tb)
		{
			case InventoryType::Id::ETC:
				fixed_tab = 3;
				break;
			case InventoryType::Id::SETUP:
				fixed_tab = 4;
				break;
		}

		return Point<int16_t>(10 + ((fixed_tab - 1) * 31), 29);
	}

	uint16_t UIItemInventory::button_by_tab(InventoryType::Id tb) const
	{
		switch (tb)
		{
			case InventoryType::Id::EQUIP:
				return Buttons::BT_TAB_EQUIP;
			case InventoryType::Id::USE:
				return Buttons::BT_TAB_USE;
			case InventoryType::Id::SETUP:
				return Buttons::BT_TAB_SETUP;
			case InventoryType::Id::ETC:
				return Buttons::BT_TAB_ETC;
			case InventoryType::Id::CASH:
				return Buttons::BT_TAB_CASH;
			default:
				return Buttons::BT_TAB_DEC;
		}
	}

	Icon* UIItemInventory::get_icon(int16_t slot)
	{
		auto iter = icons.find(slot);

		if (iter != icons.end())
			return iter->second.get();
		else
			return nullptr;
	}

	void UIItemInventory::set_full(bool enabled)
	{
		full_enabled = enabled;

		if (full_enabled)
		{
			dimension = bg_full_dimensions;

			buttons[Buttons::BT_FULL]->set_active(false);
			buttons[Buttons::BT_SMALL]->set_active(true);
		}
		else
		{
			dimension = bg_dimensions;

			buttons[Buttons::BT_FULL]->set_active(true);
			buttons[Buttons::BT_SMALL]->set_active(false);
		}

		dragarea = Point<int16_t>(dimension.x(), 20);

		buttons[Buttons::BT_CLOSE]->set_position(Point<int16_t>(dimension.x() - 20, 6));

		buttons[Buttons::BT_COIN]->set_active(!enabled);
		buttons[Buttons::BT_POINT]->set_active(!enabled);
		buttons[Buttons::BT_UPGRADE]->set_active(!enabled);
		buttons[Buttons::BT_APPRAISE]->set_active(!enabled);
		buttons[Buttons::BT_EXTRACT]->set_active(!enabled);
		buttons[Buttons::BT_DISASSEMBLE]->set_active(!enabled);
		buttons[Buttons::BT_TOAD]->set_active(!enabled);
		buttons[Buttons::BT_CASHSHOP]->set_active(!enabled);

		buttons[Buttons::BT_COIN_SM]->set_active(enabled);
		buttons[Buttons::BT_POINT_SM]->set_active(enabled);
		buttons[Buttons::BT_UPGRADE_SM]->set_active(enabled);
		buttons[Buttons::BT_APPRAISE_SM]->set_active(enabled);
		buttons[Buttons::BT_EXTRACT_SM]->set_active(enabled);
		buttons[Buttons::BT_DISASSEMBLE_SM]->set_active(enabled);
		buttons[Buttons::BT_TOAD_SM]->set_active(enabled);
		buttons[Buttons::BT_CASHSHOP]->set_active(enabled);

		set_sort(sort_enabled);
		load_icons();
	}

	void UIItemInventory::ItemIcon::set_count(int16_t c)
	{
		count = c;
	}

	Icon::IconType UIItemInventory::ItemIcon::get_type()
	{
		return Icon::IconType::ITEM;
	}

	UIItemInventory::ItemIcon::ItemIcon(const UIItemInventory& parent, InventoryType::Id st, EquipSlot::Id eqs, int16_t s, int32_t iid, int16_t c, bool u, bool cash) : parent(parent)
	{
		sourcetab = st;
		eqsource = eqs;
		source = s;
		item_id = iid;
		count = c;
		untradable = u;
		cashitem = cash;
	}

	void UIItemInventory::ItemIcon::drop_on_stage() const
	{
		constexpr const char* dropmessage = "How many will you drop?";
		constexpr const char* untradablemessage = "This item can't be taken back once thrown away.\\nWill you still drop it?";
		constexpr const char* cashmessage = "You can't drop this item.";

		if (cashitem)
		{
			UI::get().emplace<UIOk>(cashmessage, [](bool) {});
		}
		else
		{
			if (untradable)
			{
				auto onok = [&, dropmessage](bool ok)
				{
					if (ok)
					{
						if (count <= 1)
						{
							MoveItemPacket(sourcetab, source, 0, 1).dispatch();
						}
						else
						{
							auto onenter = [&](int32_t qty)
							{
								MoveItemPacket(sourcetab, source, 0, qty).dispatch();
							};

							UI::get().emplace<UIEnterNumber>(dropmessage, onenter, count, count);
						}
					}
				};

				UI::get().emplace<UIYesNo>(untradablemessage, onok);
			}
			else
			{
				if (count <= 1)
				{
					MoveItemPacket(sourcetab, source, 0, 1).dispatch();
				}
				else
				{
					auto onenter = [&](int32_t qty)
					{
						MoveItemPacket(sourcetab, source, 0, qty).dispatch();
					};

					UI::get().emplace<UIEnterNumber>(dropmessage, onenter, count, count);
				}
			}
		}
	}

	void UIItemInventory::ItemIcon::drop_on_equips(EquipSlot::Id eqslot) const
	{
		switch (sourcetab)
		{
			case InventoryType::Id::EQUIP:
			{
				if (eqsource == eqslot)
					if (parent.can_wear_equip(source))
						EquipItemPacket(source, eqslot).dispatch();

				Sound(Sound::Name::DRAGEND).play();
				break;
			}
			case InventoryType::Id::USE:
			{
				ScrollEquipPacket(source, eqslot).dispatch();
				break;
			}
		}
	}

	bool UIItemInventory::ItemIcon::drop_on_items(InventoryType::Id tab, EquipSlot::Id, int16_t slot, bool) const
	{
		if (tab != sourcetab || slot == source)
			return true;

		MoveItemPacket(tab, source, slot, 1).dispatch();

		return true;
	}

	void UIItemInventory::ItemIcon::drop_on_bindings(Point<int16_t> cursorposition, bool remove) const
	{
		if (sourcetab == InventoryType::Id::USE || sourcetab == InventoryType::Id::SETUP)
		{
			auto keyconfig = UI::get().get_element<UIKeyConfig>();
			Keyboard::Mapping mapping = Keyboard::Mapping(KeyType::ITEM, item_id);

			if (remove)
				keyconfig->unstage_mapping(mapping);
			else
				keyconfig->stage_mapping(cursorposition, mapping);
		}
	}
}