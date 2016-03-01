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
#pragma once
#include "UILoginNotice.h"
#include "IO\Components\MapleButton.h"
#include "Graphics\Sprite.h"
#include "nlnx\nx.hpp"

namespace IO
{
	UILoginNotice::UILoginNotice(int8_t text)
	{
		node notice = nl::nx::ui["Login.img"]["Notice"];

		using std::string;
		using Graphics::Sprite;
		string back = (text == 16) ? "1" : "0";
		sprites.push_back(Sprite(notice["backgrnd"][back], Point<int16_t>()));
		sprites.push_back(Sprite(notice["text"][std::to_string(text)], Point<int16_t>(17, 13)));

		buttons[BT_OK] = unique_ptr<Button>(new MapleButton(notice["BtYes"], Point<int16_t>(100, 100)));

		position = Point<int16_t>(292, 200);
		dimension = Point<int16_t>(362, 219);
		active = true;
	}

	void UILoginNotice::buttonpressed(uint16_t)
	{
		active = false;
	}
}