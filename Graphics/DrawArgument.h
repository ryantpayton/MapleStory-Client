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
#include <cstdint>

namespace Graphics
{
	class DrawArgument
	{
	public:
		DrawArgument(int16_t x, int16_t y)
		{
			pos = vector2d<int16_t>(x, y);
			defaults();
		}

		DrawArgument(vector2d<int16_t> p)
		{
			pos = p;
			defaults();
		}

		DrawArgument(vector2d<int16_t> p, float a)
		{
			pos = p;
			defaults();
			alpha = a;
		}

		DrawArgument(vector2d<int16_t> p, float xs, float ys)
		{
			pos = p;
			defaults();
			xscale = xs;
			yscale = ys;
		}

		DrawArgument(vector2d<int16_t> p, vector2d<int16_t> str)
		{
			pos = p;
			defaults();
			stretch = str;
		}

		DrawArgument(vector2d<int16_t> p, bool flip)
		{
			pos = p;
			defaults();
			xscale = flip ? -1.0f : 1.0f;
		}

		DrawArgument(vector2d<int16_t> p, bool flip, vector2d<int16_t> c)
		{
			pos = p;
			defaults();
			xscale = flip ? -1.0f : 1.0f;
			center = c;
		}

		DrawArgument(vector2d<int16_t> p, vector2d<int16_t> c, 
			vector2d<int16_t> s, float xs, float ys, float a) {

			pos = p;
			center = c;
			stretch = s;
			xscale = xs;
			yscale = ys;
			alpha = a;
		}

		~DrawArgument() {}

		vector2d<int16_t> getpos() const 
		{ 
			return pos; 
		}

		vector2d<int16_t> getcenter() const
		{
			return center;
		}

		vector2d<int16_t> getstretch() const 
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
		void defaults()
		{
			center = pos;
			xscale = 1.0f;
			yscale = 1.0f;
			alpha = 1.0f;
		}

		vector2d<int16_t> pos;
		vector2d<int16_t> center;
		vector2d<int16_t> stretch;
		float xscale;
		float yscale;
		float alpha;
	};
}