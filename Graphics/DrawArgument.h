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
#include "Util\Point.h"
#include <cstdint>

namespace Graphics
{
	class DrawArgument
	{
	public:
		DrawArgument(int16_t x, int16_t y)
			: DrawArgument(Point<int16_t>(x, y)) {}

		DrawArgument(Point<int16_t> p)
			: DrawArgument(p, 1.0f) {}

		DrawArgument(Point<int16_t> p, float xs, float ys)
			: DrawArgument(p, p, xs, ys, 1.0f) {}

		DrawArgument(Point<int16_t> p, Point<int16_t> s)
			: DrawArgument(p, p, s, 1.0f, 1.0f, 1.0f) {}

		DrawArgument(Point<int16_t> p, bool flip)
			: DrawArgument(p, flip, 1.0f) {}

		DrawArgument(Point<int16_t> p, float a)
			: DrawArgument(p, false, a) {}

		DrawArgument(Point<int16_t> p, bool flip, float a)
			: DrawArgument(p, p, flip ? -1.0f : 1.0f, 1.0f, a) {}

		DrawArgument(Point<int16_t> p, bool flip, Point<int16_t> c)
			: DrawArgument(p, c, flip ? -1.0f : 1.0f, 1.0f, 1.0f) {}

		DrawArgument(Point<int16_t> p, Point<int16_t> c,
			float xs, float ys, float a)
			: DrawArgument(p, c, Point<int16_t>(), xs, ys, a) {}

		DrawArgument(Point<int16_t> p, Point<int16_t> c, 
			Point<int16_t> s, float xs, float ys, float a) {

			pos = p;
			center = c;
			stretch = s;
			xscale = xs;
			yscale = ys;
			alpha = a;
		}

		~DrawArgument() {}

		Point<int16_t> getpos() const
		{ 
			return pos; 
		}

		Point<int16_t> getcenter() const
		{
			return center;
		}

		Point<int16_t> getstretch() const
		{
			return stretch;
		}

		float getxscale() const 
		{ 
			return xscale; 
		}

		float getyscale() const 
		{ 
			return yscale; 
		}

		float getalpha() const 
		{ 
			return alpha; 
		}

		DrawArgument overwritealpha(float newalpha) const
		{
			return DrawArgument(pos, center, stretch, xscale, yscale, newalpha);
		}

	private:
		Point<int16_t> pos;
		Point<int16_t> center;
		Point<int16_t> stretch;
		float xscale;
		float yscale;
		float alpha;
	};
}