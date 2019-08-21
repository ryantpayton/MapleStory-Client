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

#include "Point.h"
#include "Range.h"

namespace ms
{
	template <class T>
	class Rectangle
	{
	public:
		Rectangle(nl::node nlt, nl::node nrb) : lt(nlt), rb(nrb) {}
		Rectangle(nl::node src) : lt(src["lt"]), rb(src["rb"]) {}

		constexpr Rectangle(Point<T> lt, Point<T> rb) : lt(lt), rb(rb) {}
		constexpr Rectangle(T l, T r, T t, T b) : lt(l, t), rb(r, b) {}

		constexpr Rectangle() {}

		constexpr T width() const
		{
			return std::abs(lt.x() - rb.x());
		}

		constexpr T height() const
		{
			return std::abs(lt.y() - rb.y());
		}

		constexpr T l() const
		{
			return lt.x();
		}

		constexpr T t() const
		{
			return lt.y();
		}

		constexpr T r() const
		{
			return rb.x();
		}

		constexpr T b() const
		{
			return rb.y();
		}

		constexpr bool contains(const Point<T>& v) const
		{
			return !straight() && v.x() >= lt.x() && v.x() <= rb.x() && v.y() >= lt.y() && v.y() <= rb.y();
		}

		constexpr bool overlaps(const Rectangle<T>& ar) const
		{
			return Range<T>(lt.x(), rb.x()).overlaps(Range<T>(ar.lt.x(), ar.rb.x())) && Range<T>(lt.y(), rb.y()).overlaps(Range<T>(ar.lt.y(), ar.rb.y()));
		}

		constexpr bool straight() const
		{
			return lt == rb;
		}

		constexpr bool empty() const
		{
			return lt.straight() && rb.straight() && straight();
		}

		constexpr const Point<T>& getlt() const
		{
			return lt;
		}

		constexpr const Point<T>& getrb() const
		{
			return rb;
		}

		constexpr Range<T> get_horizontal() const
		{
			return { lt.x(), rb.x() };
		}

		constexpr Range<T> get_vertical() const
		{
			return { lt.y(), rb.y() };
		}

		void shift(const Point<T>& v)
		{
			lt = lt + v;
			rb = rb + v;
		}

	private:
		Point<T> lt;
		Point<T> rb;
	};
}