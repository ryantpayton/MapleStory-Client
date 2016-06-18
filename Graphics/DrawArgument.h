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
#pragma once
#include "..\Util\Point.h"

#include <cstdint>

namespace jrc
{
	class DrawArgument
	{
	public:
		constexpr DrawArgument()
			: DrawArgument(0, 0) {}

		constexpr DrawArgument(int16_t x, int16_t y)
			: DrawArgument(Point<int16_t>(x, y)) {}

		constexpr DrawArgument(Point<int16_t> p)
			: DrawArgument(p, 1.0f) {}

		constexpr DrawArgument(Point<int16_t> p, float xs, float ys)
			: DrawArgument(p, p, xs, ys, 1.0f) {}

		constexpr DrawArgument(Point<int16_t> p, Point<int16_t> s)
			: DrawArgument(p, p, s, 1.0f, 1.0f, 1.0f, 0.0f) {}

		constexpr DrawArgument(Point<int16_t> p, bool flip)
			: DrawArgument(p, flip, 1.0f) {}

		constexpr DrawArgument(float ang, Point<int16_t> p, float opc)
			: DrawArgument(ang, p, false, opc) {}

		constexpr DrawArgument(Point<int16_t> p, float opc)
			: DrawArgument(p, false, opc) {}

		constexpr DrawArgument(Point<int16_t> p, bool flip, float opc)
			: DrawArgument(p, p, flip ? -1.0f : 1.0f, 1.0f, opc) {}

		constexpr DrawArgument(float ang, Point<int16_t> p, bool flip, float opc)
			: DrawArgument(p, p, {}, flip ? -1.0f : 1.0f, 1.0f, opc, ang) {}

		constexpr DrawArgument(Point<int16_t> p, bool flip, Point<int16_t> c)
			: DrawArgument(p, c, flip ? -1.0f : 1.0f, 1.0f, 1.0f) {}

		constexpr DrawArgument(Point<int16_t> p, Point<int16_t> c,
			float xs, float ys, float opc)
			: DrawArgument(p, c, {}, xs, ys, opc, 0.0f) {}

		constexpr DrawArgument(bool flip)
			: DrawArgument(flip ? -1.0f : 1.0f, 1.0f, 1.0f) {}

		constexpr DrawArgument(float xs, float ys, float opc)
			: DrawArgument({}, xs, ys, opc) {}

		constexpr DrawArgument(Point<int16_t> p, float xs, float ys, float opc)
			: DrawArgument(p, p, xs, ys, opc) {}

		constexpr DrawArgument(Point<int16_t> p, Point<int16_t> c,
			Point<int16_t> s, float xs, float ys, float opc, float ang)
			: pos(p), center(c), stretch(s), xscale(xs), yscale(ys), opacity(opc), angle(ang) {}

		constexpr Point<int16_t> getpos() const
		{ 
			return pos; 
		}

		constexpr Point<int16_t> getcenter() const
		{
			return center;
		}

		constexpr Point<int16_t> getstretch() const
		{
			return stretch;
		}

		constexpr float getxscale() const
		{ 
			return xscale; 
		}

		constexpr float getyscale() const
		{ 
			return yscale; 
		}

		constexpr float getopacity() const
		{ 
			return opacity; 
		}

		constexpr float getangle() const
		{
			return angle;
		}

		constexpr DrawArgument operator + (Point<int16_t> argpos) const
		{
			return{
				pos + argpos, 
				center + argpos, 
				stretch, xscale, yscale, opacity, angle 
			};
		}

		constexpr DrawArgument operator + (float argopc) const
		{
			return{
				pos, center, stretch, xscale, yscale,
				opacity * argopc,
				angle 
			};
		}

		constexpr DrawArgument operator + (const DrawArgument& args) const
		{
			return{
				pos + args.pos,
				center + args.center,
				stretch + args.stretch,
				xscale * args.xscale,
				yscale * args.yscale,
				opacity * args.opacity,
				angle + args.angle
			};
		}

		constexpr DrawArgument operator - (const DrawArgument& args) const
		{
			return{
				pos - args.pos,
				center - args.center,
				stretch - args.stretch,
				xscale / args.xscale,
				yscale / args.yscale,
				opacity / args.opacity,
				angle / args.angle
			};
		}

	private:
		Point<int16_t> pos;
		Point<int16_t> center;
		Point<int16_t> stretch;
		float xscale;
		float yscale;
		float opacity;
		float angle;
	};
}