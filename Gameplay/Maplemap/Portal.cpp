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
	Portal::Portal(const Animation* a, PtType t, string nm, bool intermap, 
		Point<int16_t> p, int32_t tid, string tnm) {

		animation = a;
		type = t;
		name = nm;
		position = p;

		WarpInfo wi;
		wi.mapid = tid;
		wi.portal = intermap ? tnm : nm;
		wi.valid = tid < 999999999;
		warpinfo = wi;

		touched = false;
	}

	Portal::Portal() {}

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

	Portal::PtType Portal::gettype() const
	{
		return type;
	}

	Point<int16_t> Portal::getposition() const
	{
		return position;
	}

	rectangle2d<int16_t> Portal::bounds() const
	{
		return rectangle2d<int16_t>(
			position, 
			position + Point<int16_t>(5, -50)
			);
	}

	const WarpInfo& Portal::getwarpinfo() const
	{
		return warpinfo;
	}
}
