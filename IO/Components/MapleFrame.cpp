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
		int16_t width = (rwidth / xtile + 2) * xtile;
		int16_t height = (rheight / ytile) * ytile;
		int16_t numhor = width / xtile;
		int16_t numver = height / ytile;
		int16_t left = position.x() - width / 2;
		int16_t top = position.y() - height;

		northwest.draw({ left, top });
		southwest.draw({ left, top + height });
		west.draw({ { left, top }, { 0, ytile * numver } });
		center.draw({ { left, top }, { xtile * numhor, ytile * numver } });
		east.draw({ { left + width, top }, { 0, ytile * numver } });
		north.draw({ { left, top }, { xtile * numhor, 0 } });
		south.draw({ { left, top + height },{ xtile * numhor, 0 } });
		northeast.draw({ left + width, top });
		southeast.draw({ left + width, top + height });
	}
}