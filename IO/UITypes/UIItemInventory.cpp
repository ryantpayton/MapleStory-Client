/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
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
#include "Configuration.h"

#include "IO\UI.h"
#include "IO\Components\MapleButton.h"
#include "IO\Components\TwoSpriteButton.h"

#include "Net\Session.h"
#include "Net\Packets\InventoryPackets.h"

#include "Gameplay\Stage.h"

#include "nlnx\nx.hpp"

namespace IO
{
	using Net::Session;
	using Gameplay::Stage;
	using Character::Item;
	using Character::ItemData;
	using Character::Equipslot;

	UIItemInventory::UIItemInventory() : 
		UIDragElement(Settings::POS_INV, vector2d<int16_t>(172, 20)), 
		inventory(Stage::get().getplayer().getinvent()) {

		node src = nl::nx::ui["UIWindow2.img"]["Item"];

		sprites.push_back(src["backgrnd"]);
		sprites.push_back(src["backgrnd2"]);
		sprites.push_back(src["backgrnd3"]);

		newitemslot = src["New"]["inventory"];
		newitemtab = src["New"]["Tab0"];
		projectile = src["activeIcon"];

		node taben = src["Tab"]["enabled"];
		node tabdis = src["Tab"]["disabled"];

		buttons[BT_TAB_EQUIP]= unique_ptr<Button>(
			new TwoSpriteButton(tabdis["0"], taben["0"], vector2d<int16_t>(-1, -4))
			);
		buttons[BT_TAB_USE]= unique_ptr<Button>(
			new TwoSpriteButton(tabdis["1"], taben["1"], vector2d<int16_t>(-1, -4))
			);
		buttons[BT_TAB_ETC]= unique_ptr<Button>(
			new TwoSpriteButton(tabdis["2"], taben["2"], vector2d<int16_t>(0, -4))
			);
		buttons[BT_TAB_SETUP]= unique_ptr<Button>(
			new TwoSpriteButton(tabdis["3"], taben["3"], vector2d<int16_t>(-1, -4))
			);
		buttons[BT_TAB_CASH]= unique_ptr<Button>(
			new TwoSpriteButton(tabdis["4"], taben["4"], vector2d<int16_t>(-1, -4))
			);

		buttons[BT_DROPMESO]= unique_ptr<Button>(new MapleButton(src["BtCoin"]));
		buttons[BT_POINTS]= unique_ptr<Button>(new MapleButton(src["BtPoint0"]));
		buttons[BT_GATHER]= unique_ptr<Button>(new MapleButton(src["BtGather"]));
		buttons[BT_SORT]= unique_ptr<Button>(new MapleButton(src["BtSort"]));
		buttons[BT_EXPAND]= unique_ptr<Button>(new MapleButton(src["BtFull"]));
		buttons[BT_ITEMPOT]= unique_ptr<Button>(new MapleButton(src["BtPot3"]));
		buttons[BT_UPGRADE]= unique_ptr<Button>(new MapleButton(src["BtUpgrade3"]));
		buttons[BT_MAGNIFY]= unique_ptr<Button>(new MapleButton(src["BtAppraise3"]));
		buttons[BT_BITCASE]= unique_ptr<Button>(new MapleButton(src["BtBits3"]));

		tab = Inventory::EQUIP;
		slotrange.first = 1;
		slotrange.second = 24;
		newtab = Inventory::NONE;
		newslot = 0;

		buttons[BT_SORT]->setactive(false);
		buttons[buttonbytab(tab)]->setstate(Button::PRESSED);

		mesolabel = Text(Text::A11M, Text::RIGHT, Text::DARKGREY);
		slider = Slider(nl::nx::ui["Basic.img"]["VScr11"], vector2d<int16_t>(50, 248), 152);

		dimension = vector2d<int16_t>(172, 335);
		active = true;

		loadicons();
	}

	void UIItemInventory::draw(float inter) const
	{
		UIElement::draw(inter);

		slider.draw(position);

		for (auto& icon : icons)
		{
			int16_t slot = icon.first;
			if (slot >= slotrange.first && slot <= slotrange.second)
			{
				vector2d<int16_t> slotpos = getslotpos(slot);
				icon.second.draw(position + slotpos);
			}
		}

		int16_t pslot = inventory.getprojectile();
		if (tab == Inventory::USE && pslot > 0)
		{
			vector2d<int16_t> pslotpos = position + getslotpos(pslot);
			projectile.draw(pslotpos);
		}
		else if (newtab == tab && newslot > 0)
		{
			vector2d<int16_t> newslotpos = position + getslotpos(newslot);
			newslotpos.shifty(1);
			newitemslot.draw(newslotpos, inter);
		}
		
		if (newtab != tab && newtab != Inventory::NONE)
		{
			vector2d<int16_t> newtabpos = position + gettabpos(newtab);
			newitemtab.draw(newtabpos, inter);
		}

		vector2d<int16_t> mesopos = position + vector2d<int16_t>(124, 264);
		mesolabel.draw(mesopos);

		eqtooltip.draw(cursorposition);
		ittooltip.draw(cursorposition);
	}

	void UIItemInventory::update()
	{
		UIElement::update();

		newitemtab.update(3);
		newitemslot.update(4);

		mesolabel.settext(getmesostr());
	}

	void UIItemInventory::updateslot(int16_t slot)
	{
		Optional<Item> item = inventory.getitem(tab, slot);
		if (item)
		{
			Optional<Texture> texture = item
				.transform(&Item::getidata)
				.transform(&ItemData::geticon, false);

			if (texture)
			{
				int16_t displaycount = (tab == Inventory::EQUIP) ? 0 : item.map(&Item::getcount);
				icons[slot] = Icon(*texture, ITEMINVENTORY, slot, displaycount);
			}
		}
		else if (icons.count(slot))
		{
			icons.erase(slot);
		}
	}

	void UIItemInventory::loadicons()
	{
		icons.clear();

		uint8_t numslots = inventory.getslots(tab);
		for (uint8_t i = 1; i < numslots; i++)
		{
			updateslot(i);
		}
	}

	void UIItemInventory::buttonpressed(uint16_t buttonid)
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
			using Net::GatherItemsPacket;
			Net::Session::get().dispatch(GatherItemsPacket(tab));
			break;
		case BT_SORT:
			using Net::SortItemsPacket;
			Net::Session::get().dispatch(SortItemsPacket(tab));
			break;
		}

		if (tab != oldtab)
		{
			slotrange.first = 1;
			slotrange.second = 24;

			loadicons();

			buttons[buttonbytab(oldtab)]->setstate(Button::NORMAL);
			enablegather();
		}
	}

	void UIItemInventory::doubleclick(vector2d<int16_t> cursorpos)
	{
		int16_t slot = slotbypos(cursorpos - position);
		if (icons.count(slot) && slot >= slotrange.first && slot <= slotrange.second)
		{
			Optional<Item> item = inventory.getitem(tab, slot);
			if (item)
			{
				int32_t itemid = item.map(&Item::getid);
				switch (tab)
				{
				case Inventory::EQUIP:
					using Net::EquipItemPacket;
					Session::get().dispatch(EquipItemPacket(slot, inventory.findequipslot(itemid)));
					break;
				case Inventory::USE:
					using Net::UseItemPacket;
					Session::get().dispatch(UseItemPacket(slot, itemid));
					break;
				}
			}
		}
	}

	void UIItemInventory::icondropped(int16_t identifier)
	{
		if (!icons.count(identifier))
			return;

		using Net::MoveItemPacket;
		Session::get().dispatch(MoveItemPacket(tab, identifier, 0, 1));
	}

	void UIItemInventory::dropicon(vector2d<int16_t> cursorpos, Type type, int16_t identifier)
	{
		int16_t slot = slotbypos(cursorpos - position);
		if (slot > 0)
		{
			switch (type)
			{
			case ITEMINVENTORY:
				using Net::MoveItemPacket;
				Session::get().dispatch(MoveItemPacket(tab, identifier, slot, 1));
				break;
			case EQUIPINVENTORY:
				if (tab == Inventory::EQUIP)
				{
					Optional<Item> item = inventory.getitem(Inventory::EQUIP, slot);
					if (item)
					{
						int32_t itemid = item.map(&Item::getid);
						Equipslot::Value eqslot = inventory.findequipslot(itemid);
						if (identifier == eqslot)
						{
							using Net::EquipItemPacket;
							Session::get().dispatch(EquipItemPacket(identifier, eqslot));
						}
					}
					else
					{
						using Net::UnequipItemPacket;
						Session::get().dispatch(UnequipItemPacket(identifier, slot));
					}
				}
				break;
			}
		}
	}

	Cursor::State UIItemInventory::sendmouse(bool pressed, vector2d<int16_t> cursorpos)
	{
		cursorposition = cursorpos;

		int16_t slot = slotbypos(cursorpos - position);
		if (icons.count(slot) && slot >= slotrange.first && slot <= slotrange.second)
		{
			vector2d<int16_t> slotpos = getslotpos(slot);
			if (pressed)
			{
				icons[slot].startdrag(cursorpos - position - slotpos);
				eqtooltip.setequip(nullptr, 0);
				ittooltip.setitem(0);
				UI::get().dragicon(&icons[slot]);
				return Cursor::GRABBING;
			}
			else if (tab == Inventory::EQUIP)
			{
				Optional<Equip> equip = inventory.getequip(tab, slot);
				if (equip)
				{
					eqtooltip.setequip(equip.get(), slot);
					return Cursor::CANGRAB;
				}
				else
				{
					return Cursor::IDLE;
				}
			}
			else
			{
				int32_t itemid = inventory.getitem(tab, slot)
					.mapordefault(&Item::getid, 0);
				ittooltip.setitem(itemid);
				return Cursor::CANGRAB;
			}
		}
		else
		{
			eqtooltip.setequip(nullptr, 0);
			ittooltip.setitem(0);
			return UIDragElement::sendmouse(pressed, cursorpos);
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
			case 0:
			case 3:
				updateslot(slot);
				break;
			case 1:
				if (icons.count(slot))
					icons[slot].setcount(arg);
				break;
			case 2:
				if (arg != slot)
				{
					updateslot(slot);
					updateslot(arg);
				}
				break;
			}
		}

		switch (mode)
		{
		case 0:
		case 1:
			newtab = type;
			newslot = slot;
			break;
		case 2:
		case 3:
			if (newslot == slot && newtab == type)
			{
				newslot = 0;
				newtab = Inventory::NONE;
			}
			break;
		}
	}

	void UIItemInventory::enablesort()
	{
		buttons[BT_GATHER]->setactive(false);
		buttons[BT_SORT]->setactive(true);
		buttons[BT_SORT]->setstate(Button::NORMAL);
	}

	void UIItemInventory::enablegather()
	{
		buttons[BT_SORT]->setactive(false);
		buttons[BT_GATHER]->setactive(true);
		buttons[BT_GATHER]->setstate(Button::NORMAL);
	}

	void UIItemInventory::togglehide()
	{
		ittooltip.setitem(0);
		eqtooltip.setequip(nullptr, 0);
		UIElement::togglehide();
	}

	string UIItemInventory::getmesostr() const
	{
		string meso = std::to_string(inventory.getmeso());
		size_t pos = meso.size();
		while (pos > 3)
		{
			meso.insert(pos - 3, 1, ',');
			pos -= 3;
		}
		return meso;
	}

	int16_t UIItemInventory::slotbypos(vector2d<int16_t> cursorpos) const
	{
		int16_t xoff = cursorpos.x() - 11;
		int16_t yoff = cursorpos.y() - 51;
		if (xoff < 0 || yoff < 0)
			return 0;

		int16_t slot = (xoff / 36) + 1 + 4 * (yoff / 35);
		if (slot < slotrange.first || slot > slotrange.second)
			return 0;

		return slot;
	}

	vector2d<int16_t> UIItemInventory::getslotpos(int16_t slot) const
	{
		return vector2d<int16_t>(
			11 + ((slot - 1) % 4) * 36, 
			51 + ((slot - 1) / 4) * 35
			);
	}

	vector2d<int16_t> UIItemInventory::gettabpos(Inventory::Type tb) const
	{
		switch (tb)
		{
		case Inventory::EQUIP:
			return vector2d<int16_t>(10, 28);
		case Inventory::USE:
			return vector2d<int16_t>(42, 28);
		case Inventory::SETUP:
			return vector2d<int16_t>(74, 28);
		case Inventory::ETC:
			return vector2d<int16_t>(105, 28);
		case Inventory::CASH:
			return vector2d<int16_t>(138, 28);
		default:
			return vector2d<int16_t>();
		}
	}

	uint16_t UIItemInventory::buttonbytab(Inventory::Type tb) const
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
}