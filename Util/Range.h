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
#include <string>

using std::string;

template <class T>
class Range
{
public:
	// Construct a range from the specified values.
	Range(T first, T second)
	{
		a = first;
		b = second;
	}

	// Construct a range of (0, 0).
	Range()
	{
		a = 0;
		b = 0;
	}

	// Return the first value.
	T first() const
	{
		return a;
	}

	// Return the second value.
	T second() const
	{
		return b;
	}

	// Return the greater value.
	T greater() const
	{
		return (a > b) ? a : b;
	}

	// Return the smaller value.
	T smaller() const
	{
		return (a < b) ? a : b;
	}

	// Return the absolute difference between the values.
	T length() const
	{
		long d = std::abs(a - b);
		return static_cast<T>(d);
	}

	// Return the difference between the values.
	T delta() const
	{
		return b - a;
	}

	// Return the mean of both values.
	T center() const
	{
		return (a + b) / 2;
	}

	// Return a string representation of the range.
	string tostring() const
	{
		return "(" + std::to_string(a) + "," + std::to_string(b) + ")";
	}

	// Check if both values are equal.
	bool empty() const
	{
		return a == b;
	}

	// Check if the range contains a value.
	bool contains(T v) const
	{
		return v >= a && v <= b;
	}

	// Check if the ranges overlap.
	bool overlaps(Range<T> v) const
	{
		T c = v.first();
		T d = v.second();
		return (b >= c && a <= c) || (a <= d && b >= d) || (a >= c && b <= d) || (a <= c && b >= d);
	}

	// Set the first value.
	void setfirst(T v)
	{
		a = v;
	}

	// Set the second value.
	void setsecond(T v)
	{
		b = v;
	}

	// Shift the first value.
	void shiftfirst(T v)
	{
		a += v;
	}

	// Shift the second value.
	void shiftsecond(T v)
	{
		b += v;
	}

	// Check wether the range is equivalent to another range.
	bool operator == (Range<T> v) const
	{
		return a == v.first() && b == v.second();
	}

	// Check wether the range is not equivalent to another range.
	bool operator != (Range<T> v) const
	{
		return a != v.first() || b != v.second();
	}

	// Shift this range by the amounts defined by another range.
	void operator += (Range<T> v)
	{
		a += v.x();
		b += v.y();
	}

	// Shift this range by the negative amounts defined by another range.
	void operator -= (Range<T> v)
	{
		a -= v.x();
		b -= v.y();
	}

	// Return the negative of this range.
	Range<T> operator - ()
	{
		return Range<T>(-a, -b);
	}

private:
	T a;
	T b;
};