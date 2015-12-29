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
#include "UIItemInventory.h"
#include "IO\UI.h"
#include "IO\Components\MapleButton.h"
#include "IO\Components\TwoSpriteButton.h"
#include "Program\Configuration.h"
#include "Net\Session.h"
#include "Net\Packets\GameplayPackets83.h"
#include "nlnx\nx.hpp"

namespace IO
{
	UIItemInventory::UIItemInventory(const Inventory& inv) : inventory(inv)
	{
		tab = Inventory::EQUIP;
		slotrange.first = 1;
		slotrange.second = 24;

		node src = nl::nx::ui["UIWindow2.img"]["Item"];

		sprites.push_back(Sprite(src["backgrnd"], vector2d<int16_t>()));
		sprites.push_back(Sprite(src["backgrnd2"], vector2d<int16_t>()));
		sprites.push_back(Sprite(src["backgrnd3"], vector2d<int16_t>()));

		newitemslot = Animation(src["New"]["inventory"]);
		newitemtab = Animation(src["New"]["Tab0"]);

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

		buttons[BT_SORT]->setactive(false);

		buttons[buttonbytab(tab)]->setstate(Button::PRESSED);

		mesolabel = Textlabel(Textlabel::DWF_12MR, Textlabel::TXC_BLACK, "", 0);

		position = Program::Configuration::getvector2d("PosINV");
		dimension = vector2d<int16_t>(172, 335);
		active = true;

		dragarea = vector2d<int16_t>(172, 20);
		cursoroffset = vector2d<int16_t>();
		dragged = false;

		newtab = Inventory::NONE;
		newslot = 0;

		loadicons();
	}

	UIItemInventory::~UIItemInventory()
	{
		Program::Configuration::setstring("PosINV", position.tostring());
	}

	void UIItemInventory::draw(float inter) const
	{
		UIElement::draw(inter);

		for (auto& icon : icons)
		{
			int16_t slot = icon.first;
			if (slot >= slotrange.first && slot <= slotrange.second)
			{
				vector2d<int16_t> slotpos = getslotpos(slot);
				icon.second.draw(position + slotpos);
			}
		}

		using Graphics::DrawArgument;
		if (newtab == tab && newslot > 0)
		{
			newitemslot.draw(
				DrawArgument(position + getslotpos(newslot) + vector2d<int16_t>(0, 1)), inter
				);
		}
		else if (newtab != Inventory::NONE)
		{
			newitemtab.draw(
				DrawArgument(position + gettabpos(newtab)), inter
				);
		}

		mesolabel.drawline(getmesostr(), position + vector2d<int16_t>(124, 262));
		eqtooltip.draw(cursorposition);
	}

	void UIItemInventory::update()
	{
		UIElement::update();

		newitemtab.update(3);
		newitemslot.update(4);
	}

	void UIItemInventory::addicon(int16_t slot)
	{
		using Character::Item;
		using Character::ItemData;

		const Item* item = inventory.getitem(tab, slot);
		if (item == nullptr)
			return;

		const ItemData& idata = item->getidata();
		icons[slot] = Icon(idata.geticon(false), Element::ITEMINVENTORY, slot, item->getcount());
	}

	void UIItemInventory::loadicons()
	{
		icons.clear();

		uint8_t numslots = inventory.getslots(tab);
		for (uint8_t i = 0; i < numslots; i++)
		{
			addicon(i);
		}
	}

	void UIItemInventory::buttonpressed(uint16_t buttonid)
	{
		Inventory::InvType oldtab = tab;
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
		}

		if (tab != oldtab)
		{
			slotrange.first = 1;
			slotrange.second = 24;

			loadicons();

			buttons[buttonbytab(oldtab)]->setstate(Button::NORMAL);
		}
	}

	void UIItemInventory::doubleclick(vector2d<int16_t> cursorpos)
	{
		int16_t slot = slotbypos(cursorpos - position);
		if (icons.count(slot) && slot >= slotrange.first && slot <= slotrange.second)
		{
			using Character::Item;
			const Item* item = inventory.getitem(tab, slot);
			if (item == nullptr)
				return;

			switch (tab)
			{
			case Inventory::EQUIP:
				using Net::EquipItemPacket;
				Net::Session::dispatch(EquipItemPacket(slot, inventory.findequipslot(item->getid())));
				break;
			case Inventory::USE:
				using Net::UseItemPacket83;
				Net::Session::dispatch(UseItemPacket83(slot, item->getid()));
				break;
			}
		}
	}

	Cursor::Mousestate UIItemInventory::sendmouse(bool pressed, vector2d<int16_t> cursorpos)
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
				UI::dragicon(&icons[slot]);
				return Cursor::MST_GRABBING;
			}
			else
			{
				const Equip* equip = inventory.getequip(tab, slot);
				if (equip)
					eqtooltip.setequip(equip, slot);

				return Cursor::MST_CANGRAB;
			}
		}
		else
		{
			eqtooltip.setequip(nullptr, 0);
			return UIDragElement::sendmouse(pressed, cursorpos);
		}
	}

	void UIItemInventory::modify(Inventory::InvType type, int16_t slot, int8_t mode, int16_t arg)
	{
		if (slot == 0)
			return;

		if (type == tab)
		{
			switch (mode)
			{
			case 0:
				addicon(slot);
				break;
			case 1:
				if (icons.count(slot))
				{
					icons[slot].setcount(arg);
				}
				break;
			case 2:
				if (icons.count(slot))
				{
					icons[arg] = icons[slot];
					icons.erase(slot);
				}
				break;
			case 3:
				icons.erase(slot);
				break;
			}
		}

		if (mode == 0 || mode == 1)
		{
			newslot = slot;
			newtab = type;
		}
		else if (newslot == slot && newtab == type)
		{
			newslot = 0;
			newtab = Inventory::NONE;
		}
	}

	void UIItemInventory::togglehide()
	{
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

		return (xoff / 36) + 1 + 4 * (yoff / 35);
	}

	vector2d<int16_t> UIItemInventory::getslotpos(int16_t slot) const
	{
		return vector2d<int16_t>(
			11 + ((slot - 1) % 4) * 36, 
			51 + ((slot - 1) / 4) * 35
			);
	}

	vector2d<int16_t> UIItemInventory::gettabpos(Inventory::InvType tb) const
	{
		switch (tb)
		{
		case Inventory::EQUIP:
			return vector2d<int16_t>(10, 28);
		case Inventory::USE:
			return vector2d<int16_t>(42, 28);
		case Inventory::ETC:
			return vector2d<int16_t>(74, 28);
		case Inventory::SETUP:
			return vector2d<int16_t>(105, 28);
		case Inventory::CASH:
			return vector2d<int16_t>(138, 28);
		default:
			return vector2d<int16_t>();
		}
	}

	uint16_t UIItemInventory::buttonbytab(Inventory::InvType tb) const
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