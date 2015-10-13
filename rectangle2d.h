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

namespace Util
{
	template <class T>
	class rectangle2d
	{
	public:
		rectangle2d(T l, T r, T t, T b) { lt = vector2d<T>(l, t); rb = vector2d<T>(r, b); }
		rectangle2d(vector2d<T> v1, vector2d<T> v2) { lt = v1; rb = v2; }
		rectangle2d() { lt = vector2d<T>(); rb = vector2d<T>(); }
		~rectangle2d() {}
		T getwidth() { return abs(lt.x() - rb.x()); }
		T l() { return lt.x(); }
		T t() { return lt.y(); }
		T r() { return rb.x(); }
		T b() { return rb.y(); }
		bool contains(vector2d<T> v) 
		{ 
			return !straight() && v.x() >= lt.x() && v.x() <= rb.x() && v.y() >= lt.y() && v.y() <= rb.y(); 
		}
		bool overlaps(rectangle2d<T> ar)
		{
			vector2d<T> rhor = vector2d<T>(lt.x(), rb.x());
			vector2d<T> rver = vector2d<T>(lt.y(), rb.y());
			vector2d<T> ahor = vector2d<T>(ar.getlt().x(), ar.getrb().x());
			vector2d<T> aver = vector2d<T>(ar.getlt().y(), ar.getrb().y());
			return rhor.overlaps(ahor) && rver.overlaps(aver);
		}
		bool straight() { return lt == rb; }
		void setlt(vector2d<T> l) { lt = l; }
		void setrb(vector2d<T> r) { rb = r; }
		void shift(vector2d<T> v) { lt = lt + v; rb = rb + v; }
		void shiftlt(vector2d<T> l) { lt = lt + l; }
		void shiftrb(vector2d<T> r) { rb = rb + r; }
		vector2d<T> getlt() { return lt; }
		vector2d<T> getrb() { return rb; }
	private:
		vector2d<T> lt;
		vector2d<T> rb;
	};
}

