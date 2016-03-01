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

	UIEquipInventory::UIEquipInventory() :
		UIDragElement(Settings::POS_EQINV, Point<int16_t>(184, 20)),
		inventory(Stage::get().getplayer().getinvent()) {

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
			if (iconpositions.count(slot))
				icit.second.draw(position + iconpositions.at(slot));
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

		for (auto& icon : iconpositions)
		{
			int16_t slot = icon.first;
			addicon(slot);
		}
	}

	Cursor::State UIEquipInventory::sendmouse(bool pressed, Point<int16_t> cursorpos)
	{
		cursorposition = cursorpos;

		int16_t slot = slotbypos(cursorpos);
		if (icons.count(slot))
		{
			if (pressed)
			{
				tooltip.clear();
				icons[slot].startdrag(cursorpos - position - iconpositions[slot]);
				UI::get().dragicon(&icons[slot]);
				return Cursor::GRABBING;
			}
			else
			{
				Optional<Equip> equip = inventory.getequip(Inventory::EQUIPPED, slot);
				if (equip)
					tooltip.setequip(equip.get(), slot);

				return Cursor::CANGRAB;
			}
		}

		tooltip.clear();
		return UIDragElement::sendmouse(pressed, cursorpos);
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
				Session::get().dispatch(UnequipItemPacket(slot, freeslot));
			}
		}
	}

	void UIEquipInventory::icondropped(int16_t identifier)
	{
		using Net::UnequipItemPacket;
		Session::get().dispatch(UnequipItemPacket(identifier, 0));
	}

	void UIEquipInventory::dropicon(Point<int16_t> cursorpos, Type type, int16_t identifier)
	{
		int16_t slot = slotbypos(cursorpos);
		if (slot > 0)
		{
			Equipslot::Value eqslot;
			switch (type)
			{
			case ITEMINVENTORY:
				eqslot = inventory.findequipslot(
					inventory.getitem(Inventory::EQUIP, identifier)
					.mapordefault(&Item::getid, 0)
					);
				break;
			default:
				eqslot = Equipslot::NONE;
			}

			if (slot == eqslot)
			{
				using Net::EquipItemPacket;
				Session::get().dispatch(EquipItemPacket(identifier, eqslot));
			}
		}
	}

	void UIEquipInventory::togglehide()
	{
		tooltip.clear();
		UIElement::togglehide();
	}

	void UIEquipInventory::addicon(int16_t slot)
	{
		Optional<const Texture> texture = inventory.getitem(Inventory::EQUIPPED, slot)
			.transform(&Item::getidata)
			.transform(&ItemData::geticon, false);

		if (texture)
			icons[slot] = Icon(*texture, EQUIPINVENTORY, slot, 0);
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
}