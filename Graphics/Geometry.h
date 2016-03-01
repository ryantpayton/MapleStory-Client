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
#include <cstdint>

namespace Graphics
{
	class Geometry
	{
	public:
		virtual ~Geometry() {}

	protected:
		static const size_t NUM_COLORS = 5;
		enum Color
		{
			// Common
			BLACK,
			WHITE,

			// Mob hp bar
			HPBAR_LIGHTGREEN,
			HPBAR_GREEN,
			HPBAR_DARKGREEN
		};

		void draw(int16_t x, int16_t y, int16_t w, int16_t h, Geometry::Color color, float opacity) const;
	};
}