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
#include "vector2d.h"
#include <cstdint>

namespace Graphics
{
	using::std::int32_t;
	using::Util::vector2d;

	class DrawArgument
	{
	public:
		virtual ~DrawArgument() {}
		virtual vector2d<int32_t> getpos() const { return vector2d<int32_t>(); }
		virtual vector2d<int32_t> getstretch() const { return vector2d<int32_t>(); }
		virtual vector2d<int32_t> getcenter() const { return getpos(); }
		virtual float getxscale() const { return 1.0f; }
		virtual float getyscale() const { return 1.0f; }
		virtual float getalpha() const { return 1.0f; }
	};

	class PosArgument : public DrawArgument
	{
	public:
		PosArgument(vector2d<int32_t> p) : pos(p) {}
		vector2d<int32_t> getpos() const { return pos; }
	private:
		vector2d<int32_t> pos;
	};

	class FlipArgument : public DrawArgument
	{
	public:
		FlipArgument(vector2d<int32_t> p, bool f, vector2d<int32_t> c) : pos(p), xscale(f ? -1.0f : 1.0f), center(c) {}
		FlipArgument(vector2d<int32_t> p, bool f) : pos(p), xscale(f ? -1.0f : 1.0f), center(p) {}
		vector2d<int32_t> getpos() const { return pos; }
		float getxscale() const { return xscale; }
		vector2d<int32_t> getcenter() const { return center; }
	private:
		vector2d<int32_t> pos;
		float xscale;
		vector2d<int32_t> center;
	};

	class AlphaArgument : public DrawArgument
	{
	public:
		AlphaArgument(vector2d<int32_t> p, float a) : pos(p), alpha(a) {}
		vector2d<int32_t> getpos() const { return pos; }
		float getalpha() const { return alpha; }
	private:
		vector2d<int32_t> pos;
		float alpha;
	};

	class StretchArgument : public DrawArgument
	{
	public:
		StretchArgument(vector2d<int32_t> p, vector2d<int32_t> s) : pos(p), stretch(s) {}
		vector2d<int32_t> getpos() const { return pos; }
		vector2d<int32_t> getstretch() const { return stretch; }
	private:
		vector2d<int32_t> pos;
		vector2d<int32_t> stretch;
	};

	class FullArgument : public DrawArgument
	{
	public:
		FullArgument(vector2d<int32_t> p, vector2d<int32_t> s, vector2d<int32_t> c, float xs, float ys, float a)
			: pos(p), stretch(s), center(c), xscale(xs), yscale(ys), alpha(a) {}
		vector2d<int32_t> getpos() const { return pos; }
		vector2d<int32_t> getstretch() const { return stretch; }
		vector2d<int32_t> getcenter() const { return center; }
		float getxscale() const { return xscale; }
		float getyscale() const { return yscale; }
		float getalpha() const { return alpha; }
	private:
		vector2d<int32_t> pos;
		vector2d<int32_t> stretch;
		vector2d<int32_t> center;
		float xscale;
		float yscale;
		float alpha;
	};
}