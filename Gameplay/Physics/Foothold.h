//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../../Template/Range.h"

#include <nlnx/node.hpp>

namespace ms
{
	// Represents a platform part on a maple-map.
	class Foothold
	{
	public:
		Foothold(nl::node src, std::uint16_t id, std::uint8_t layer);
		Foothold();

		// Returns the foothold id aka the identifier in game data of this platform.
		std::uint16_t id() const;
		// Returns the platform left to this.
		std::uint16_t prev() const;
		// Returns the platform right to this.
		std::uint16_t next() const;
		// Returns the platform's layer.
		std::uint8_t layer() const;
		// Returns the horizontal component.
		const Range<int16_t>& horizontal() const;
		// Returns the vertical component.
		const Range<int16_t>& vertical() const;

		// Return the left edge.
		std::int16_t l() const;
		// Return the right edge.
		std::int16_t r() const;
		// Return the top edge.
		std::int16_t t() const;
		// Return the bottom edge.
		std::int16_t b() const;
		// Return the first horizontal component.
		std::int16_t x1() const;
		// Return the second horizontal component.
		std::int16_t x2() const;
		// Return the first vertical component.
		std::int16_t y1() const;
		// Return the second vertical component.
		std::int16_t y2() const;
		// Return if the platform is a wall (x1 == x2).
		bool is_wall() const;
		// Return if the platform is a floor (y1 == y2).
		bool is_floor() const;
		// Return if this platform is a left edge.
		bool is_left_edge() const;
		// Return if this platform is a right edge.
		bool is_right_edge() const;
		// Returns if a x-coordinate is above or below this platform.
		bool hcontains(std::int16_t x) const;
		// Returns if a y-coordinate is right or left of this platform.
		bool vcontains(std::int16_t y) const;
		// Check whether this foothold blocks an object.
		bool is_blocking(const Range<int16_t>& vertical) const;
		// Returns the width.
		std::int16_t hdelta() const;
		// Returns the height.
		std::int16_t vdelta() const;
		// Returns the slope as a ratio of vertical/horizontal.
		double slope() const;
		// Returns a y-coordinate right below the given x-coordinate.
		double ground_below(double x) const;

	private:
		std::uint16_t m_id;
		std::uint16_t m_prev;
		std::uint16_t m_next;
		std::uint8_t m_layer;
		Range<int16_t> m_horizontal;
		Range<int16_t> m_vertical;
	};
}