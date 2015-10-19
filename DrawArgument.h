/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "vector2d.h"

using namespace Util;

namespace Graphics
{
	class DrawArgument
	{
	public:
		virtual ~DrawArgument() {}
		virtual vector2d<int> getpos() { return vector2d<int>(); }
		virtual vector2d<int> getstretch() { return vector2d<int>(); }
		virtual vector2d<int> getcenter() { return getpos(); }
		virtual float getxscale() { return 1.0f; }
		virtual float getyscale() { return 1.0f; }
		virtual float getalpha() { return 1.0f; }
	};

	class PosArgument : public DrawArgument
	{
	public:
		PosArgument(vector2d<int> p) : pos(p) {}
		vector2d<int> getpos() { return pos; }
	private:
		vector2d<int> pos;
	};

	class FlipArgument : public DrawArgument
	{
	public:
		FlipArgument(vector2d<int> p, bool f, vector2d<int> c) : pos(p), xscale(f ? -1.0f : 1.0f), center(c) {}
		FlipArgument(vector2d<int> p, bool f) : pos(p), xscale(f ? -1.0f : 1.0f), center(p) {}
		vector2d<int> getpos() { return pos; }
		float getxscale() { return xscale; }
		vector2d<int> getcenter() { return center; }
	private:
		vector2d<int> pos;
		float xscale;
		vector2d<int> center;
	};

	class AlphaArgument : public DrawArgument
	{
	public:
		AlphaArgument(vector2d<int> p, float a) : pos(p), alpha(a) {}
		vector2d<int> getpos() { return pos; }
		float getalpha() { return alpha; }
	private:
		vector2d<int> pos;
		float alpha;
	};

	class StretchArgument : public DrawArgument
	{
	public:
		StretchArgument(vector2d<int> p, vector2d<int> s) : pos(p), stretch(s) {}
		vector2d<int> getpos() { return pos; }
		vector2d<int> getstretch() { return stretch; }
	private:
		vector2d<int> pos;
		vector2d<int> stretch;
	};
}