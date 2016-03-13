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
#include "Portal.h"

namespace Gameplay
{
	Portal::Portal(const Animation* a, Type t, string nm, bool intramap,
		Point<int16_t> p, int32_t tid, string tnm) {

		animation = a;
		type = t;
		name = nm;
		position = p;

		warpinfo = WarpInfo(tid, intramap ? tnm : nm);

		touched = false;
	}

	Portal::Portal() 
		: Portal(nullptr, SPAWN, "", false, Point<int16_t>(), 0, "") {}

	Portal::~Portal() {}

	void Portal::draw(Point<int16_t> viewpos, float inter) const
	{
		if (!animation || (type == HIDDEN && !touched))
			return;

		animation->draw(position + viewpos, inter);
	}

	void Portal::settouch(bool t)
	{
		touched = t;
	}

	string Portal::getname() const
	{
		return name;
	}

	Portal::Type Portal::gettype() const
	{
		return type;
	}

	Point<int16_t> Portal::getposition() const
	{
		return position;
	}

	rectangle2d<int16_t> Portal::bounds() const
	{
		auto lt = position + Point<int16_t>(-25, -50);
		auto rb = position + Point<int16_t>(25, 50);
		return rectangle2d<int16_t>(lt, rb);
	}

	Portal::WarpInfo Portal::getwarpinfo() const
	{
		return warpinfo;
	}
}
