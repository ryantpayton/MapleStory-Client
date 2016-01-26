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
#include "IO\UI.h"
#include "IO\Components\MapleButton.h"
#include "Program\Configuration.h"
#include "Net\Session.h"
#include "Net\Packets\InventoryPackets.h"
#include "nlnx\nx.hpp"

namespace IO
{
	UIEquipInventory::UIEquipInventory(const Inventory& inv) :
		UIDragElement("PosEQINV", vector2d<int16_t>(184, 20)),
		inventory(inv) {

		iconpositions[1] = vector2d<int16_t>(43, 25);
		iconpositions[2] = vector2d<int16_t>(43, 91);
		iconpositions[3] = vector2d<int16_t>(43, 68);
		iconpositions[4] = vector2d<int16_t>(109, 91);
		iconpositions[5] = vector2d<int16_t>(43, 124);
		iconpositions[6] = vector2d<int16_t>(43, 157);
		iconpositions[7] = vector2d<int16_t>(76, 190);
		iconpositions[8] = vector2d<int16_t>(10, 157);
		iconpositions[9] = vector2d<int16_t>(142, 124);
		iconpositions[10] = vector2d<int16_t>(142, 124);
		iconpositions[11] = vector2d<int16_t>(109, 124);
		iconpositions[12] = vector2d<int16_t>(109, 157);
		iconpositions[13] = vector2d<int16_t>(142, 157);
		iconpositions[15] = vector2d<int16_t>(109, 91);
		iconpositions[16] = vector2d<int16_t>(142, 91);
		iconpositions[17] = vector2d<int16_t>(76, 124);
		iconpositions[18] = vector2d<int16_t>(142, 91);
		iconpositions[19] = vector2d<int16_t>(76, 124);
		iconpositions[49] = vector2d<int16_t>(10, 58);
		iconpositions[50] = vector2d<int16_t>(76, 157);

		using nl::node;
		node source = nl::nx::ui["UIWindow2.img"]["Equip"]["character"];
		node petsource = nl::nx::ui["UIWindow2.img"]["Equip"]["pet"];

		sprites.push_back(Sprite(source["backgrnd"]));
		sprites.push_back(Sprite(source["backgrnd2"]));
		sprites.push_back(Sprite(source["backgrnd3_Kanna"]));
		sprites.push_back(Sprite(source["cashPendant"]));
		sprites.push_back(Sprite(source["charmPocket"]));
		sprites.push_back(Sprite(source["emblem"]));

		buttons[BT_TOGGLEPETS] = unique_ptr<Button>(new MapleButton(source["BtPet"]));

		pettextures.push_back(Texture(petsource["backgrnd"]));
		pettextures.push_back(Texture(petsource["backgrnd2"]));
		pettextures.push_back(Texture(petsource["backgrnd3"]));

		loadicons();

		dimension = vector2d<int16_t>(184, 290);
		active = true;
		showpetequips = false;
	}

	void UIEquipInventory::draw(float inter) const
	{
		UIElement::draw(inter);

		for (auto& icit : icons)
		{
			int16_t slot = icit.first;
			if (iconpositions.count(slot))
				icit.second.draw(position + iconpositions.at(slot));
		}

		if (showpetequips)
		{
			using Graphics::DrawArgument;
			for (auto& ptit : pettextures)
			{
				ptit.draw(DrawArgument(position + vector2d<int16_t>(184, 0)));
			}
		}

		tooltip.draw(cursorposition);
	}

	void UIEquipInventory::buttonpressed(uint16_t id)
	{
		if (id == BT_TOGGLEPETS)
		{
			showpetequips = !showpetequips;
			buttons.at(id)->setstate(Button::NORMAL);
		}
	}

	void UIEquipInventory::loadicons()
	{
		icons.clear();

		for (auto& icp : iconpositions)
		{
			addicon(icp.first);
		}
	}

	Cursor::Mousestate UIEquipInventory::sendmouse(bool pressed, vector2d<int16_t> cursorpos)
	{
		cursorposition = cursorpos;

		int16_t slot = slotbypos(cursorpos);
		if (icons.count(slot))
		{
			if (pressed)
			{
				icons[slot].startdrag(cursorpos - position - iconpositions[slot]);
				tooltip.setequip(nullptr, 0);
				UI::get().dragicon(&icons[slot]);
				return Cursor::MST_GRABBING;
			}
			else
			{
				const Equip* equip = inventory.getequip(Inventory::EQUIPPED, slot);
				if (equip)
				{
					tooltip.setequip(equip, slot);
					return Cursor::MST_CANGRAB;
				}
			}
		}

		tooltip.setequip(nullptr, 0);
		return UIDragElement::sendmouse(pressed, cursorpos);
	}

	void UIEquipInventory::doubleclick(vector2d<int16_t> cursorpos)
	{
		int16_t slot = slotbypos(cursorpos);
		if (icons.count(slot))
		{
			int16_t freeslot = inventory.findslot(Inventory::EQUIP);
			if (freeslot > 0)
			{
				using Net::UnequipItemPacket;
				Net::Session::get().dispatch(UnequipItemPacket(slot, freeslot));
			}
		}
	}

	void UIEquipInventory::icondropped(int16_t identifier)
	{
		using Net::UnequipItemPacket;
		Net::Session::get().dispatch(UnequipItemPacket(identifier, 0));
	}

	void UIEquipInventory::togglehide()
	{
		tooltip.setequip(nullptr, 0);
		UIElement::togglehide();
	}

	void UIEquipInventory::addicon(int16_t slot)
	{
		using Character::Item;
		const Item* item = inventory.getitem(Inventory::EQUIPPED, slot);
		if (item == nullptr)
			return;

		icons[slot] = Icon(item->getidata().geticon(false), Element::EQUIPINVENTORY, slot, 0);
	}

	void UIEquipInventory::modify(int16_t pos, int8_t mode, int16_t arg)
	{
		if (mode != 0 && icons.count(pos) == 0)
			return;

		switch (mode)
		{
		case 0:
			addicon(pos);
			break;
		case 2:
			icons[arg] = icons[pos];
			icons.erase(pos);
			break;
		case 3:
			icons.erase(pos);
			break;
		}
	}

	int16_t UIEquipInventory::slotbypos(vector2d<int16_t> cursorpos) const
	{
		for (auto& icit : iconpositions)
		{
			int16_t slot = icit.first;
			rectangle2d<int16_t> iconrect = rectangle2d<int16_t>(
				position + icit.second,
				position + icit.second + vector2d<int16_t>(32, 32)
				);
			if (iconrect.contains(cursorpos))
			{
				return slot;
			}
		}
		return 0;
	}
}