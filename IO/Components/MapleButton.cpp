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
#include "MapleButton.h"

namespace jrc
{
	MapleButton::MapleButton(nl::node src, Point<int16_t> pos)
	{
		textures[PRESSED] = src["pressed"]["0"];
		textures[MOUSEOVER] = src["mouseOver"]["0"];
		textures[NORMAL] = src["normal"]["0"];
		textures[DISABLED] = src["disabled"]["0"];
		position = pos;
		state = NORMAL;
		active = true;
	}

	MapleButton::MapleButton(nl::node src, int16_t x, int16_t y)
		: MapleButton(src, Point<int16_t>(x, y)) {}

	MapleButton::MapleButton(nl::node src)
		: MapleButton(src, Point<int16_t>()) {}

	void MapleButton::draw(Point<int16_t> parentpos) const
	{
		if (active)
		{
			textures[state].draw(position + parentpos);
		}
	}

	Rectangle<int16_t> MapleButton::bounds(Point<int16_t> parentpos) const
	{
		auto lt = parentpos + position - textures[state].getorigin();
		auto rb = lt + textures[state].getdimensions();
		return Rectangle<int16_t>(lt, rb);
	}
}
