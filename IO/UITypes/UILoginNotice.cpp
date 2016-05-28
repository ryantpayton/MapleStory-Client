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
#include "UILoginNotice.h"

#include "..\Components\MapleButton.h"

#include "..\..\Graphics\Sprite.h"

#include <nlnx\nx.hpp>

namespace jrc
{
	UILoginNotice::UILoginNotice(int8_t text)
	{
		nl::node notice = nl::nx::ui["Login.img"]["Notice"];

		std::string back = (text == 16) ? "1" : "0";
		sprites.emplace_back(notice["backgrnd"][back]);
		sprites.emplace_back(notice["text"][std::to_string(text)], Point<int16_t>(17, 13));

		buttons[BT_OK] = std::make_unique<MapleButton>(notice["BtYes"], Point<int16_t>(100, 100));

		position = Point<int16_t>(292, 200);
		dimension = Point<int16_t>(362, 219);
		active = true;
	}

	void UILoginNotice::buttonpressed(uint16_t)
	{
		active = false;
	}
}