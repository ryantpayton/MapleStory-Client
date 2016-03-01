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
#include "nlnx\node.hpp"

using std::pair;
using std::string;
using nl::node;

template <class T>
class Point
{
public:
	// Construct a point from a vector property.
	Point(node src)
	{
		a = static_cast<T>(src.x());
		b = static_cast<T>(src.y());
	}

	// Construct a point from the specified coordinates.
	Point(T first, T second)
	{
		a = first;
		b = second;
	}

	// Construct a point with coordinates (0, 0).
	Point()
	{
		a = 0;
		b = 0;
	}

	// Return the x-coordinate.
	T x() const 
	{ 
		return a; 
	}

	// Return the y-coordinate.
	T y() const 
	{ 
		return b; 
	}

	// Return the inner product.
	T length() const 
	{ 
		double p = std::sqrt(a * a + b * b);
		return static_cast<T>(p); 
	}

	// Check wether the coordinates are equal.
	bool straight() const
	{
		return a == b;
	}

	// Return a string representation of the point.
	string tostring() const 
	{ 
		return "(" + std::to_string(a) + "," + std::to_string(b) + ")"; 
	}

	// Return the distance to another point.
	T distance(Point<T> v) const 
	{ 
		Point<T> distance = Point<T>(a - v.x(), b - v.y());
		return distance.length();
	}

	// Set the x-coordinate.
	void setx(T v) 
	{ 
		a = v; 
	}

	// Set the y-coordinate.
	void sety(T v) 
	{ 
		b = v; 
	}

	// Shift the x-coordinate by the specified amount.
	void shiftx(T v) 
	{ 
		a += v;
	}

	// Shift the y-coordinate by the specified amount.
	void shifty(T v) 
	{ 
		b += v; 
	}

	// Shift the coordinates by the specified amounts.
	void shift(T x, T y) 
	{ 
		a += x; 
		b += y; 
	}

	// Shift the this point by the amounts defined by another point. Equivalent to operator +=.
	void shift(Point<T> v) 
	{ 
		a += v.x(); 
		b += v.y(); 
	}

	// Check wether point is equivalent to the specified point.
	bool operator == (Point<T> v) const 
	{ 
		return a == v.x() && b == v.y(); 
	}

	// Check wether point is not equivalent to the specified point.
	bool operator != (Point<T> v) const
	{ 
		return a != v.x() || b != v.y(); 
	}

	// Shift the this point by the amounts defined by another point.
	void operator += (Point<T> v)
	{
		a += v.x();
		b += v.y();
	}

	// Shift the this point in reverse direction by the amounts defined by another point.
	void operator -= (Point<T> v)
	{
		a -= v.x();
		b -= v.y();
	}

	// Return a point whose coordaintes are the negation of this point's coordinates.
	Point<T> operator - () 
	{ 
		return Point<T>(-a, -b);
	}

	// Return a point whose coordinates have been added the specified amount.
	Point<T> operator + (T v) const
	{
		return Point<T>(a + v, b + v);
	}

	// Return a point whose coordinates have been substracted the specified amount.
	Point<T> operator - (T v) const
	{
		return Point<T>(a - v, b - v);
	}

	// Return a point whose coordinates have been multiplied by the specified amount.
	Point<T> operator * (T v) const
	{ 
		return Point<T>(a * v, b * v);
	}

	// Return a point whose coordinates have been divided by the specified amount.
	Point<T> operator / (T v) const
	{
		return Point<T>(a / v, b / v);
	}

	// Return a point whose coordinates are the sum of this and another points coordinates.
	Point<T> operator + (Point<T> v) const
	{ 
		return Point<T>(a + v.x(), b + v.y());
	}

	// Return a point whose coordinates are the difference of this and another points coordinates.
	Point<T> operator - (Point<T> v) const
	{ 
		return Point<T>(a - v.x(), b - v.y());
	}

	// Return a point whose coordinates are the product of this and another points coordinates.
	Point<T> operator * (Point<T> v) const
	{ 
		return vector2d<T>(a / v.x(), b / v.y()); 
	}

	// Return a point whose coordinates are the division of this and another points coordinates.
	Point<T> operator / (Point<T> v) const
	{ 
		return vector2d<T>(a / ((v.x() == 0) ? 1 : v.x()), b / ((v.y() == 0) ? 1 : v.y())); 
	}

private:
	T a;
	T b;
};