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
#include "Point.h"
#include "Range.h"

template <class T>
class rectangle2d
{
public:
	rectangle2d(node src) { lt = src["lt"]; rb = src["rb"]; }
	rectangle2d(node nlt, node nrb) { lt = nlt; rb = nrb; }
	rectangle2d(T l, T r, T t, T b) { lt = Point<T>(l, t); rb = Point<T>(r, b); }
	rectangle2d(Point<T> v1, Point<T> v2) { lt = v1; rb = v2; }
	rectangle2d() { lt = Point<T>(); rb = Point<T>(); }
	~rectangle2d() {}
	T width() const { return abs(lt.x() - rb.x()); }
	T height() const { return abs(lt.y() - rb.y()); }
	T l() const { return lt.x(); }
	T t() const { return lt.y(); }
	T r() const { return rb.x(); }
	T b() const { return rb.y(); }
	bool contains(Point<T> v) const { return !straight() && v.x() >= lt.x() && v.x() <= rb.x() && v.y() >= lt.y() && v.y() <= rb.y(); }
	bool overlaps(rectangle2d<T> ar) const
	{
		Range<T> rhor = Range<T>(lt.x(), rb.x());
		Range<T> rver = Range<T>(lt.y(), rb.y());
		Range<T> ahor = Range<T>(ar.getlt().x(), ar.getrb().x());
		Range<T> aver = Range<T>(ar.getlt().y(), ar.getrb().y());
		return rhor.overlaps(ahor) && rver.overlaps(aver);
	}
	bool straight() const { return lt == rb; }
	bool empty() const { return lt.straight() && rb.straight() && straight(); }
	Point<T> getlt() const { return lt; }
	Point<T> getrb() const { return rb; }
	Range<T> gethor() const { return Range<T>(lt.x(), rb.x()); }
	Range<T> getver() const { return Range<T>(lt.y(), rb.y()); }
	void setlt(Point<T> l) { lt = l; }
	void setrb(Point<T> r) { rb = r; }
	void shift(Point<T> v) { lt = lt + v; rb = rb + v; }
	void shiftlt(Point<T> l) { lt = lt + l; }
	void shiftrb(Point<T> r) { rb = rb + r; }

private:
	Point<T> lt;
	Point<T> rb;
};

