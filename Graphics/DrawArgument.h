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
		DrawArgument()
			: DrawArgument(0, 0) {}

		DrawArgument(int16_t x, int16_t y)
			: DrawArgument(Point<int16_t>(x, y)) {}

		DrawArgument(Point<int16_t> p)
			: DrawArgument(p, 1.0f) {}

		DrawArgument(Point<int16_t> p, float xs, float ys)
			: DrawArgument(p, p, xs, ys, 1.0f) {}

		DrawArgument(Point<int16_t> p, Point<int16_t> s)
			: DrawArgument(p, p, s, 1.0f, 1.0f, 1.0f, 0.0f) {}

		DrawArgument(Point<int16_t> p, bool flip)
			: DrawArgument(p, flip, 1.0f) {}

		DrawArgument(float ang, Point<int16_t> p, float opc)
			: DrawArgument(ang, p, false, opc) {}

		DrawArgument(Point<int16_t> p, float opc)
			: DrawArgument(p, false, opc) {}

		DrawArgument(Point<int16_t> p, bool flip, float opc)
			: DrawArgument(p, p, flip ? -1.0f : 1.0f, 1.0f, opc) {}

		DrawArgument(float ang, Point<int16_t> p, bool flip, float opc)
			: DrawArgument(p, p, Point<int16_t>(), flip ? -1.0f : 1.0f, 1.0f, opc, ang) {}

		DrawArgument(Point<int16_t> p, bool flip, Point<int16_t> c)
			: DrawArgument(p, c, flip ? -1.0f : 1.0f, 1.0f, 1.0f) {}

		DrawArgument(Point<int16_t> p, Point<int16_t> c,
			float xs, float ys, float opc)
			: DrawArgument(p, c, Point<int16_t>(), xs, ys, opc, 0.0f) {}

		DrawArgument(bool flip)
			: DrawArgument(flip ? -1.0f : 1.0f, 1.0f, 1.0f) {}

		DrawArgument(float xs, float ys, float opc)
			: DrawArgument(Point<int16_t>(), xs, ys, opc) {}

		DrawArgument(Point<int16_t> p, float xs, float ys, float opc)
			: DrawArgument(p, p, xs, ys, opc) {}

		DrawArgument(Point<int16_t> p, Point<int16_t> c, 
			Point<int16_t> s, float xs, float ys, float opc, float ang) {

			pos = p;
			center = c;
			stretch = s;
			xscale = xs;
			yscale = ys;
			opacity = opc;
			angle = ang;
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

		float getopacity() const 
		{ 
			return opacity; 
		}

		float getangle() const
		{
			return angle;
		}

		DrawArgument operator + (Point<int16_t> argpos) const
		{
			auto psum = pos + argpos;
			auto csum = center + argpos;
			return DrawArgument(psum, csum, stretch, xscale, yscale, opacity, angle);
		}

		DrawArgument operator + (float argopc) const
		{
			auto opcsum = opacity * argopc;
			return DrawArgument(pos, center, stretch, xscale, yscale, opcsum, angle);
		}

		DrawArgument operator + (const DrawArgument& args) const
		{
			auto psum = pos + args.pos;
			auto csum = center + args.center;
			auto ssum = stretch + args.stretch;
			auto xssum = xscale * args.xscale;
			auto yssum = yscale * args.yscale;
			auto opcsum = opacity * args.opacity;
			auto anglesum = angle + args.angle;
			return DrawArgument(psum, csum, ssum, xssum, yssum, opcsum, anglesum);
		}

		DrawArgument operator - (const DrawArgument& args) const
		{
			auto pdiff = pos - args.pos;
			auto cdiff = center - args.center;
			auto sdiff = stretch - args.stretch;
			auto xsdiff = xscale / args.xscale;
			auto ysdiff = yscale / args.yscale;
			auto opcdiff = opacity / args.opacity;
			auto anglediff = angle / args.angle;
			return DrawArgument(pdiff, cdiff, sdiff, xsdiff, ysdiff, opcdiff, anglediff);
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