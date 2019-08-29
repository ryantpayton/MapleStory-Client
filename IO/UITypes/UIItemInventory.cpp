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
#include "../Components/TwoSpriteButton.h"
#include "../Data/ItemData.h"
#include "../Audio/Audio.h"
#include "../Character/Player.h"
#include "../Gameplay/Stage.h"
#include "../Data/EquipData.h"

#include "../Net/Packets/InventoryPackets.h"

#include <nlnx/nx.hpp>

namespace ms
{
	UIItemInventory::UIItemInventory(const Inventory& invent) : UIDragElement<PosINV>(Point<int16_t>(172, 20)), inventory(invent), ignore_tooltip(false)
	{
		nl::node close = nl::nx::ui["Basic.img"]["BtClose"];
		nl::node src = nl::nx::ui["UIWindow2.img"]["Item"];

		sprites.emplace_back(src["backgrnd"]); // TODO: productionBackgrnd not working
		sprites.emplace_back(src["productionBackgrnd2"]);
		sprites.emplace_back(src["backgrnd3"]); // TODO: productionBackgrnd3 not working

		newitemslot = src["New"]["inventory"];
		newitemtab = src["New"]["Tab0"];
		projectile = src["activeIcon"];

		nl::node taben = src["Tab"]["enabled"];
		nl::node tabdis = src["Tab"]["disabled"];

		buttons[Buttons::BT_CLOSE] = std::make_unique<MapleButton>(close, Point<int16_t>(156, 12));

		buttons[Buttons::BT_TAB_EQUIP] = std::make_unique<TwoSpriteButton>(tabdis["0"], taben["0"]);
		buttons[Buttons::BT_TAB_USE] = std::make_unique<TwoSpriteButton>(tabdis["1"], taben["1"]);
		buttons[Buttons::BT_TAB_ETC] = std::make_unique<TwoSpriteButton>(tabdis["2"], taben["2"]);
		buttons[Buttons::BT_TAB_SETUP] = std::make_unique<TwoSpriteButton>(tabdis["3"], taben["3"]);
		buttons[Buttons::BT_TAB_CASH] = std::make_unique<TwoSpriteButton>(tabdis["4"], taben["4"]);

		buttons[Buttons::BT_DROPMESO] = std::make_unique<MapleButton>(src["BtCoin3"]);
		buttons[Buttons::BT_POINTS] = std::make_unique<MapleButton>(src["BtPoint0"]);
		buttons[Buttons::BT_GATHER] = std::make_unique<MapleButton>(src["BtGather3"]);
		buttons[Buttons::BT_SORT] = std::make_unique<MapleButton>(src["BtSort3"]);
		buttons[Buttons::BT_EXPAND] = std::make_unique<MapleButton>(src["BtFull3"]);
		buttons[Buttons::BT_ITEMPOT] = std::make_unique<MapleButton>(src["BtPot3"]);
		buttons[Buttons::BT_UPGRADE] = std::make_unique<MapleButton>(src["BtUpgrade3"]);
		buttons[Buttons::BT_MAGNIFY] = std::make_unique<MapleButton>(src["BtAppraise3"]);
		buttons[Buttons::BT_EXTRACT] = std::make_unique<MapleButton>(src["BtExtract3"]);
		buttons[Buttons::BT_DISASSEMBLE] = std::make_unique<MapleButton>(src["BtDisassemble3"]);
		buttons[Buttons::BT_TOAD] = std::make_unique<MapleButton>(src["BtToad3"]);

		tab = InventoryType::Id::EQUIP;
		slotrange.first = 1;
		slotrange.second = 24;

		buttons[Buttons::BT_SORT]->set_active(false);
		buttons[Buttons::BT_ITEMPOT]->set_state(Button::State::DISABLED);
		buttons[Buttons::BT_EXTRACT]->set_state(Button::State::DISABLED);
		buttons[Buttons::BT_DISASSEMBLE]->set_state(Button::State::DISABLED);
		buttons[button_by_tab(tab)]->set_state(Button::State::PRESSED);

		mesolabel = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::BLACK);
		maplepointslabel = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::BLACK);

		slider = Slider(
			Slider::Type::DEFAULT, Range<int16_t>(50, 245), 152, 6, 1 + inventory.get_slotmax(tab) / 4,
			[&](bool upwards)
			{
				int16_t shift = upwards ? -4 : 4;
				bool above = slotrange.first + shift > 0;
				bool below = slotrange.second + shift < inventory.get_slotmax(tab) + 1 + 4;

				if (above && below)
				{
					slotrange.first += shift;
					slotrange.second += shift;
				}
			}
		);

		dimension = Point<int16_t>(172, 335);
		active = true;

		clear_new();
		load_icons();
	}

	void UIItemInventory::draw(float alpha) const
	{
		UIElement::draw(alpha);

		slider.draw(position + Point<int16_t>(0, 1));

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

		if (tab == InventoryType::Id::USE && is_visible(bulletslot))
		{
			Point<int16_t> bulletslotpos = position + get_slotpos(bulletslot);
			projectile.draw(DrawArgument(bulletslotpos));
		}
		else if (newtab == tab && is_visible(newslot))
		{
			Point<int16_t> newslotpos = position + get_slotpos(newslot);
			newslotpos.shift(Point<int16_t>(1, 1));
			newitemslot.draw(DrawArgument(newslotpos), alpha);
		}

		if (newtab != InventoryType::Id::NONE)
		{
			Point<int16_t> newtabpos = position + get_tabpos(newtab);
			newitemtab.draw(DrawArgument(newtabpos), alpha);
		}

		mesolabel.draw(position + Point<int16_t>(127, 262));
		maplepointslabel.draw(position + Point<int16_t>(159, 279));
	}

	void UIItemInventory::update()
	{
		UIElement::update();

		newitemtab.update(6);
		newitemslot.update(6);

		int64_t meso = inventory.get_meso();
		std::string mesostr = std::to_string(meso);
		string_format::split_number(mesostr);
		mesolabel.change_text(mesostr);
		maplepointslabel.change_text("0"); // TODO: Need to bring this value in
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
			Equipslot::Id eqslot = inventory.find_equipslot(item_id);

			icons[slot] = std::make_unique<Icon>(
				std::make_unique<ItemIcon>(*this, tab, eqslot, slot, count, untradable, cashitem),
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

		for (uint8_t i = 1; i < numslots; i++)
			update_slot(i);
	}

	Button::State UIItemInventory::button_pressed(uint16_t buttonid)
	{
		InventoryType::Id oldtab = tab;

		switch (buttonid)
		{
		case Buttons::BT_CLOSE:
			toggle_active();
			return Button::State::NORMAL;
		case Buttons::BT_TAB_EQUIP:
			tab = InventoryType::Id::EQUIP;
			break;
		case Buttons::BT_TAB_USE:
			tab = InventoryType::Id::USE;
			break;
		case Buttons::BT_TAB_SETUP:
			tab = InventoryType::Id::SETUP;
			break;
		case Buttons::BT_TAB_ETC:
			tab = InventoryType::Id::ETC;
			break;
		case Buttons::BT_TAB_CASH:
			tab = InventoryType::Id::CASH;
			break;
		case Buttons::BT_GATHER:
			GatherItemsPacket(tab).dispatch();
			break;
		case Buttons::BT_SORT:
			SortItemsPacket(tab).dispatch();
			break;
		case Buttons::BT_DROPMESO:
		case Buttons::BT_POINTS:
		case Buttons::BT_EXPAND:
		case Buttons::BT_ITEMPOT:
		case Buttons::BT_UPGRADE:
		case Buttons::BT_MAGNIFY:;
		case Buttons::BT_EXTRACT:
		case Buttons::BT_DISASSEMBLE:
		case Buttons::BT_TOAD:
			return Button::State::NORMAL;
		}

		if (tab != oldtab)
		{
			slotrange.first = 1;
			slotrange.second = 24;

			slider.setrows(6, 1 + inventory.get_slotmax(tab) / 4);

			buttons[button_by_tab(oldtab)]->set_state(Button::State::NORMAL);
			buttons[button_by_tab(tab)]->set_state(Button::State::PRESSED);

			load_icons();
			enable_gather();
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
					if (can_wear_equip(slot))
						EquipItemPacket(slot, inventory.find_equipslot(item_id)).dispatch();

					break;
				case InventoryType::Id::USE:
					UseItemPacket(slot, item_id).dispatch();
					break;
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
			Equipslot::Id eqslot;
			bool equip;

			if (item_id && tab == InventoryType::Id::EQUIP)
			{
				eqslot = inventory.find_equipslot(item_id);
				equip = true;
			}
			else
			{
				eqslot = Equipslot::Id::NONE;
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

		if (slider.isenabled())
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

				InventoryType::Id oldtab = tab;

				switch (oldtab)
				{
				case InventoryType::Id::EQUIP:
					tab = InventoryType::Id::USE;
					break;
				case InventoryType::Id::USE:
					tab = InventoryType::Id::ETC;
					break;
				case InventoryType::Id::ETC:
					tab = InventoryType::Id::SETUP;
					break;
				case InventoryType::Id::SETUP:
					tab = InventoryType::Id::CASH;
					break;
				case InventoryType::Id::CASH:
					tab = InventoryType::Id::EQUIP;
					break;
				}

				if (tab != oldtab)
				{
					slotrange.first = 1;
					slotrange.second = 24;

					slider.setrows(6, 1 + inventory.get_slotmax(tab) / 4);

					buttons[button_by_tab(oldtab)]->set_state(Button::State::NORMAL);

					load_icons();
					enable_gather();
				}

				buttons[button_by_tab(tab)]->set_state(Button::State::PRESSED);
			}
		}
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
				update_slot(slot);
				newtab = type;
				newslot = slot;
				break;
			case Inventory::Modification::CHANGECOUNT:
			case Inventory::Modification::ADDCOUNT:
				if (auto icon = get_icon(slot))
					icon->set_count(arg);

				break;
			case Inventory::Modification::SWAP:
				if (arg != slot)
				{
					update_slot(slot);
					update_slot(arg);
				}

				break;
			case Inventory::Modification::REMOVE:
				update_slot(slot);
				break;
			}
		}

		switch (mode)
		{
		case Inventory::Modification::ADD:
		case Inventory::Modification::ADDCOUNT:
			newtab = type;
			newslot = slot;
			break;
		case Inventory::Modification::CHANGECOUNT:
		case Inventory::Modification::SWAP:
		case Inventory::Modification::REMOVE:
			if (newslot == slot && newtab == type)
				clear_new();

			break;
		}
	}

	void UIItemInventory::enable_sort()
	{
		buttons[Buttons::BT_GATHER]->set_active(false);
		buttons[Buttons::BT_SORT]->set_active(true);
		buttons[Buttons::BT_SORT]->set_state(Button::State::NORMAL);
	}

	void UIItemInventory::enable_gather()
	{
		buttons[Buttons::BT_SORT]->set_active(false);
		buttons[Buttons::BT_GATHER]->set_active(true);
		buttons[Buttons::BT_GATHER]->set_state(Button::State::NORMAL);
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

	bool UIItemInventory::remove_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (UIDragElement::remove_cursor(clicked, cursorpos))
			return true;

		return slider.remove_cursor(clicked);
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
		return slot < slotrange.first || slot > slotrange.second;
	}

	bool UIItemInventory::can_wear_equip(int16_t slot) const
	{
		const Player& player = Stage::get().get_player();
		const CharStats& stats = player.get_stats();
		const CharLook& look = player.get_look();
		const bool alerted = look.get_alerted();

		if (alerted)
		{
			UI::get().emplace<UIOk>("You cannot complete this action right now.\\nEvade the attack and try again.", []() {});
			return false;
		}

		const int32_t item_id = inventory.get_item_id(InventoryType::Id::EQUIP, slot);
		const EquipData& equipdata = EquipData::get(item_id);
		const ItemData& itemdata = equipdata.get_itemdata();

		const int8_t reqGender = itemdata.get_gender();
		const bool female = stats.get_female();

		switch (reqGender)
		{
		case 0: // Male
			if (female)
				return false;

			break;
		case 1: // Female
			if (!female)
				return false;

			break;
		case 2: // Unisex
		default:
			break;
		}

		const std::string jobname = stats.get_jobname();

		if (jobname == "GM" || jobname == "SuperGM")
			return true;

		int16_t reqJOB = equipdata.get_reqstat(Maplestat::Id::JOB);

		if (!stats.get_job().is_sub_job(reqJOB))
		{
			UI::get().emplace<UIOk>("Your current job\\ncannot equip the selected item.", []() {});
			return false;
		}

		int16_t reqLevel = equipdata.get_reqstat(Maplestat::Id::LEVEL);
		int16_t reqDEX = equipdata.get_reqstat(Maplestat::Id::DEX);
		int16_t reqSTR = equipdata.get_reqstat(Maplestat::Id::STR);
		int16_t reqLUK = equipdata.get_reqstat(Maplestat::Id::LUK);
		int16_t reqINT = equipdata.get_reqstat(Maplestat::Id::INT);
		int16_t reqFAME = equipdata.get_reqstat(Maplestat::Id::FAME);

		int8_t i = 0;

		if (reqLevel > stats.get_stat(Maplestat::Id::LEVEL))
			i++;
		else if (reqDEX > stats.get_total(Equipstat::Id::DEX))
			i++;
		else if (reqSTR > stats.get_total(Equipstat::Id::STR))
			i++;
		else if (reqLUK > stats.get_total(Equipstat::Id::LUK))
			i++;
		else if (reqINT > stats.get_total(Equipstat::Id::INT))
			i++;
		else if (reqFAME > stats.get_honor())
			i++;

		if (i > 0)
		{
			UI::get().emplace<UIOk>("Your stats are too low to equip this item\\nor you do not meet the job requirement.", []() {});
			return false;
		}

		return true;
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
			10 + (absslot % 4) * 36,
			51 + (absslot / 4) * 35
			);
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
		default:
			return Buttons::BT_TAB_CASH;
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

	void UIItemInventory::ItemIcon::set_count(int16_t c)
	{
		count = c;
	}

	UIItemInventory::ItemIcon::ItemIcon(const UIItemInventory& parent, InventoryType::Id st, Equipslot::Id eqs, int16_t s, int16_t c, bool u, bool cash) : parent(parent)
	{
		sourcetab = st;
		eqsource = eqs;
		source = s;
		count = c;
		untradable = u;
		cashitem = cash;
	}

	void UIItemInventory::ItemIcon::drop_on_stage() const
	{
		constexpr char* dropmessage = "How many will you drop?";
		constexpr char* untradablemessage = "This item can't be taken back once thrown away.\\nWill you still drop it?";
		constexpr char* cashmessage = "You can't drop this item.";

		if (cashitem)
		{
			UI::get().emplace<UIOk>(cashmessage, []() {});
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

	void UIItemInventory::ItemIcon::drop_on_equips(Equipslot::Id eqslot) const
	{
		switch (sourcetab)
		{
		case InventoryType::Id::EQUIP:
			if (eqsource == eqslot)
				if (parent.can_wear_equip(source))
					EquipItemPacket(source, eqslot).dispatch();

			Sound(Sound::Name::DRAGEND).play();

			break;
		case InventoryType::Id::USE:
			ScrollEquipPacket(source, eqslot).dispatch();
			break;
		}
	}

	bool UIItemInventory::ItemIcon::drop_on_items(InventoryType::Id tab, Equipslot::Id, int16_t slot, bool) const
	{
		if (tab != sourcetab || slot == source)
			return true;

		MoveItemPacket(tab, source, slot, 1).dispatch();

		return true;
	}
}