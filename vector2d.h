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
#include <string>

using namespace std;

namespace Util
{
	template <class T>
	class vector2d
	{
	public:
		vector2d() { a = 0; b = 0; }
		vector2d(T v1, T v2) { a = v1; b = v2; }
		~vector2d() {}
		T x() { return a; }
		T y() { return b; }
		T length() { return static_cast<T>(std::sqrt(std::pow(a, 2) + std::pow(b, 2))); }
		T center() { return ((a > b) ? a - b : b - a) / 2; }
		bool contains(T p) { return (p >= a && p <= b); }
		bool straight() { return a == b; }
		bool overlaps(vector2d<T> v) { return (b >= v.x() && a <= v.x()) || (a <= v.y() && b >= v.y()) || (a >= v.x() && b <= v.y()) || (a <= v.x() && b >= v.y()); }
		string tostring() { return to_string(a) + "," + to_string(b); }
		vector2d<T> operator + (vector2d<T> v) { return vector2d<T>(a + v.x(), b + v.y()); }
		vector2d<T> operator - (vector2d<T> v) { return vector2d<T>(a - v.x(), b - v.y()); }
		vector2d<T> operator * (vector2d<T> v) { return vector2d<T>(a / v.x(), b / v.y()); }
		vector2d<T> operator / (vector2d<T> v) { return vector2d<T>(a / ((v.x() == 0) ? 1 : v.x()), b / ((v.y() == 0) ? 1 : v.y())); }
		bool operator == (vector2d<T> v) { return a == v.x() && b == v.y(); }
		bool operator != (vector2d<T> v) { return a != v.x() || b != v.y(); }
		void setx(T v) { a = v; }
		void sety(T v) { b = v; }
		void shiftx(T v) { a += v; }
		void shifty(T v) { b += v; }
		void shift(T x, T y) { a += x; b += y; }
		void shift(vector2d<T> v) { a += v.x(); b += v.y(); }
		void operator += (vector2d<T> v) { a += v.x(); b += v.y(); }
		void operator -= (vector2d<T> v) { a -= v.x(); b -= v.y(); }
	private:
		T a;
		T b;
	};
}

