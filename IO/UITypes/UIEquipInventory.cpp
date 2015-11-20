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
#include "IO\Components\MapleButton.h"
#include "Program\Configuration.h"
#include "Character\EquipConstants.h"
#include "nlnx\nx.hpp"

namespace IO
{
	UIEquipInventory::UIEquipInventory(const Inventory& inv) : inventory(inv)
	{
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

		dragarea = vector2d<int16_t>(184, 20);
		cursoroffset = vector2d<int16_t>();
		dragged = false;

		position = Program::Configuration::getvector2d("PosEQINV");
		dimension = vector2d<int16_t>(184, 290);
		active = true;
		showpetequips = false;
	}

	UIEquipInventory::~UIEquipInventory()
	{
		Program::Configuration::setstring("PosEQINV", position.tostring());
	}

	void UIEquipInventory::draw(float inter) const
	{
		UIElement::draw(inter);

		if (showpetequips)
		{
			Graphics::DrawArgument args = Graphics::DrawArgument(
				position + vector2d<int16_t>(184, 0)
				);

			for (auto& ptit : pettextures)
			{
				ptit.draw(args);
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

	void UIEquipInventory::loadicons()
	{

	}
}