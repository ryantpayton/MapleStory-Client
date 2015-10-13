/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "MapleButton.h"
#include "Sprite.h"
#include "nx.hpp"

namespace IO
{
	UILoginNotice::UILoginNotice(char text)
	{
		node notice = nx::ui["Login.img"]["Notice"];

		string back = (text == 16) ? "1" : "0";
		graphics.add(new Texture(notice["backgrnd"][back]));
		graphics.add(new Sprite(notice["text"][to_string(text)], vector2d<int>(17, 13)));

		buttons.add(BT_OK, new MapleButton(notice["BtYes"], vector2d<int>(100, 100)));

		position = vector2d<int>(292, 200);
		dimension = vector2d<int>(362, 219);
		active = true;
	}

	void UILoginNotice::buttonpressed(short id)
	{
		active = false;
	}
}