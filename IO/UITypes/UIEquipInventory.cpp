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
#include "UIEquipInventory.h"

#include "Gameplay\Stage.h"
#include "IO\UI.h"
#include "IO\Components\MapleButton.h"
#include "Net\Packets\InventoryPackets.h"

#include "nlnx\nx.hpp"

namespace IO
{
	using Gameplay::Stage;
	using Character::Item;
	using Character::ItemData;
	using Character::Equipslot;

	UIEquipInventory::UIEquipInventory() :
		UIDragElement<PosEQINV>(Point<int16_t>(184, 20)), inventory(Stage::get().getplayer().getinvent()) {

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

		using nl::node;
		node source = nl::nx::ui["UIWindow2.img"]["Equip"]["character"];
		node petsource = nl::nx::ui["UIWindow2.img"]["Equip"]["pet"];

		sprites.push_back(source["backgrnd"]);
		sprites.push_back(source["backgrnd2"]);
		sprites.push_back(source["backgrnd3_Kanna"]);
		sprites.push_back(source["cashPendant"]);
		sprites.push_back(source["charmPocket"]);
		sprites.push_back(source["emblem"]);

		buttons[BT_TOGGLEPETS] = unique_ptr<Button>(new MapleButton(source["BtPet"]));

		pettextures.push_back(petsource["backgrnd"]);
		pettextures.push_back(petsource["backgrnd2"]);
		pettextures.push_back(petsource["backgrnd3"]);

		loadicons();

		dimension = Point<int16_t>(184, 290);
		active = true;
		showpetequips = false;
	}

	void UIEquipInventory::draw(float inter) const
	{
		UIElement::draw(inter);

		for (auto& icit : icons)
		{
			int16_t slot = icit.first;
			if (icit.second && iconpositions.count(slot))
				icit.second->draw(position + iconpositions.at(slot));
		}

		if (showpetequips)
		{
			using Graphics::DrawArgument;
			for (auto& ptit : pettextures)
			{
				Point<int16_t> petposition = position + Point<int16_t>(184, 0);
				ptit.draw(petposition);
			}
		}
	}

	void UIEquipInventory::buttonpressed(uint16_t id)
	{
		if (id == BT_TOGGLEPETS)
		{
			showpetequips = !showpetequips;
			buttons.at(id)->setstate(Button::NORMAL);
		}
	}

	void UIEquipInventory::updateslot(int16_t slot)
	{
		Optional<const Texture> texture = inventory.getitem(Inventory::EQUIPPED, slot)
			.transform(&Item::getidata)
			.transform(&ItemData::geticon, false);

		if (texture)
		{
			icons[slot] = unique_ptr<Icon>(new Icon(new EquipIcon(slot), *texture, -1));
		}
		else if (icons.count(slot))
		{
			icons.erase(slot);
		}

		cleartooltip();
	}

	void UIEquipInventory::loadicons()
	{
		icons.clear();

		for (auto& icon : iconpositions)
		{
			int16_t slot = icon.first;
			updateslot(slot);
		}
	}

	Cursor::State UIEquipInventory::sendmouse(bool pressed, Point<int16_t> cursorpos)
	{
		Cursor::State dstate = UIDragElement::sendmouse(pressed, cursorpos);
		if (dragged)
		{
			cleartooltip();
			return dstate;
		}

		int16_t slot = slotbypos(cursorpos);
		Optional<Icon> icon = geticon(slot);
		if (icon)
		{
			if (pressed)
			{
				icon->startdrag(cursorpos - position - iconpositions[slot]);
				UI::get().dragicon(icon.get());

				cleartooltip();
				return Cursor::GRABBING;
			}
			else
			{
				showequip(slot);
				return Cursor::CANGRAB;
			}
		}
		else
		{
			cleartooltip();
			return Cursor::IDLE;
		}
	}

	void UIEquipInventory::doubleclick(Point<int16_t> cursorpos)
	{
		int16_t slot = slotbypos(cursorpos);
		if (icons.count(slot))
		{
			int16_t freeslot = inventory.findslot(Inventory::EQUIP);
			if (freeslot > 0)
			{
				using Net::UnequipItemPacket;
				UnequipItemPacket(slot, freeslot).dispatch();
			}
		}
	}

	void UIEquipInventory::sendicon(const Icon& icon, Point<int16_t> cursorpos)
	{
		int16_t slot = slotbypos(cursorpos);
		if (slot > 0)
		{
			Equipslot::Value eqslot = Equipslot::byvalue(slot);
			icon.droponequips(eqslot);
		}
	}

	void UIEquipInventory::togglehide()
	{
		cleartooltip();
		UIElement::togglehide();
	}

	void UIEquipInventory::modify(int16_t pos, int8_t mode, int16_t arg)
	{
		switch (mode)
		{
		case 0:
		case 3:
			updateslot(pos);
			break;
		case 2:
			updateslot(pos);
			updateslot(arg);
			break;
		}
	}

	void UIEquipInventory::showequip(int16_t slot)
	{
		Optional<Equip> equip = inventory.getequip(Inventory::EQUIPPED, slot);
		int16_t eqslot = -slot;
		UI::get().withstate(&UIState::showequip, EQUIPINVENTORY, equip.get(), eqslot);
	}

	void UIEquipInventory::cleartooltip()
	{
		UI::get().withstate(&UIState::cleartooltip, EQUIPINVENTORY);
	}

	int16_t UIEquipInventory::slotbypos(Point<int16_t> cursorpos) const
	{
		for (auto& icit : iconpositions)
		{
			int16_t slot = icit.first;
			rectangle2d<int16_t> iconrect = rectangle2d<int16_t>(
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

	void UIEquipInventory::EquipIcon::ondrop() const
	{
		using Net::UnequipItemPacket;
		UnequipItemPacket(source, 0).dispatch();
	}

	void UIEquipInventory::EquipIcon::ondropitems(Inventory::Type tab, Equipslot::Value eqslot, int16_t slot, bool equip) const
	{
		if (tab != Inventory::EQUIP)
			return;

		if (equip)
		{
			if (eqslot == source)
			{
				using Net::EquipItemPacket;
				EquipItemPacket(slot, eqslot).dispatch();
			}
		}
		else
		{
			using Net::UnequipItemPacket;
			UnequipItemPacket(source, slot).dispatch();
		}
	}
}