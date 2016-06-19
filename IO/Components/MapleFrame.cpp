/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "MapleFrame.h"

#include <nlnx\nx.hpp>

#include <algorithm>

namespace jrc
{
	MapleFrame::MapleFrame(nl::node src)
	{
		center = src["c"];
		east = src["e"];
		northeast = src["ne"];
		north = src["n"];
		northwest = src["nw"];
		west = src["w"];
		southwest = src["sw"];
		south = src["s"];
		southeast = src["se"];

		xtile = std::max<int16_t>(north.width(), 1);
		ytile = std::max<int16_t>(west.height(), 1);
	}

	MapleFrame::MapleFrame() {}

	void MapleFrame::draw(Point<int16_t> position, int16_t rwidth, int16_t rheight) const
	{
		int16_t numhor = rwidth / xtile + 2;
		int16_t numver = rheight / ytile;
		int16_t width = numhor * xtile;
		int16_t height = numver * ytile;
		int16_t left = position.x() - width / 2;
		int16_t top = position.y() - height;
		int16_t right = left + width;
		int16_t bottom = top + height;

		northwest.draw({ left, top });
		southwest.draw({ left, bottom });
		for (int16_t y = top; y < bottom; y += ytile)
		{
			west.draw({ left, y });
			east.draw({ right, y });
		}
		center.draw({ { left, top }, { width, height } });
		for (int16_t x = left; x < right; x += xtile)
		{
			north.draw({ x, top });
			south.draw({ x, bottom });
		}
		northeast.draw({ right, top });
		southeast.draw({ right, bottom });
	}
}