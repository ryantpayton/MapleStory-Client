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
	using::nl::node;
	using::Util::vector2d;

	// Represents a platform on a maple-map. Contains methods for usage with physics.
	class Foothold
	{
	public:
		// Parameters: short(foothold id), char(map layer), node(game data)
		Foothold(uint16_t, int8_t, node);
		Foothold();
		~Foothold(){}
		// Returns the foothold id aka the identifier in game data of this platform.
		uint16_t getid() const;
		// Returns the map layer. Used for drawing objects in the right order.
		int8_t getlayer() const;
		// Returns the platform left to this.
		uint16_t getprev() const;
		// Returns the platform right to this.
		uint16_t getnext() const;
		// Returns the horizontal component.
		vector2d<int32_t> gethor() const;
		// Returns the vertical component.
		vector2d<int32_t> getver() const;
		// Get coordinates.
		int32_t getl() const;
		int32_t getr() const;
		int32_t gett() const;
		int32_t getb() const;
		bool iswall() const;
		bool isfloor() const;
		// Returns if a x-coordinate is above or below this platform.
		// Parameters: int(x-coordinate to test)
		bool hcontains(int32_t) const;
		// Returns if a y-coordinate is right or left of this platform.
		// Parameters: int(y-coordinate to test)
		bool vcontains(int32_t) const;
		// Returns the width.
		int32_t gethdelta() const;
		// Returns the height.
		int32_t getvdelta() const;
		// Returns the slope as a ratio of vertical/horizontal.
		float getslope() const;
		// Returns a y-coordinate right above the given x-coordinate. Used for slopes.
		// Parameters: float(x-coordinate)
		float resolvex(float) const;
		// Returns a point slightly left off the edge of this platform.
		vector2d<int32_t> getledge() const;
		// Returns a point slightly right off the edge of this platform.
		vector2d<int32_t> getredge() const;
	private:
		uint16_t id;
		int8_t layer;
		uint16_t prev;
		uint16_t next;
		vector2d<int32_t> horizontal;
		vector2d<int32_t> vertical;
	};
}

