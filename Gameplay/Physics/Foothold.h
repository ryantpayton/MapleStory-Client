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
#include "Util\vector2d.h"
#include "nlnx\node.hpp"

namespace Gameplay
{
	using nl::node;
	using Util::vector2d;

	// Represents a platform part on a maple-map. Contains methods for usage with physics.
	class Foothold
	{
	public:
		Foothold(node src, int8_t layer);
		Foothold();
		~Foothold();

		// Returns the foothold id aka the identifier in game data of this platform.
		uint16_t getid() const { return id; }
		// Returns the map layer. Used for drawing objects in the right order.
		int8_t getlayer() const { return layer; }
		// Returns the platform left to this.
		uint16_t getprev() const { return prev; }
		// Returns the platform right to this.
		uint16_t getnext() const { return next; }
		// Returns the horizontal component.
		const vector2d<int16_t>& gethor() const { return horizontal; }
		// Returns the vertical component.
		const vector2d<int16_t>& getver() const { return vertical; }
		// Return the left edge.
		int16_t getl() const { return horizontal.smaller(); }
		// Return the right edge.
		int16_t getr() const { return horizontal.greater(); }
		// Return the top edge.
		int16_t gett() const { return vertical.smaller(); }
		// Return the bottom edge.
		int16_t getb() const { return vertical.greater(); }
		// Return if the platform is a wall (x1 == x2).
		bool iswall() const { return horizontal.straight(); }
		// Return if the platform is a floor (y1 == y2).
		bool isfloor() const { return vertical.straight(); }
		// Returns if a x-coordinate is above or below this platform.
		bool hcontains(int16_t x) const { return horizontal.contains(x); }
		// Returns if a y-coordinate is right or left of this platform.
		bool vcontains(int16_t y) const { return vertical.contains(y); }
		// Returns the width.
		int16_t gethdelta() const { return horizontal.y() - horizontal.x(); }
		// Returns the height.
		int16_t getvdelta() const { return vertical.y() - vertical.x(); }
		// Returns the slope as a ratio of vertical/horizontal.
		float getslope() const;
		// Returns a y-coordinate right above the given x-coordinate. Used for slopes.
		float resolvex(float x) const;

	private:
		uint16_t id;
		int8_t layer;
		uint16_t prev;
		uint16_t next;
		vector2d<int16_t> horizontal;
		vector2d<int16_t> vertical;
	};
}

