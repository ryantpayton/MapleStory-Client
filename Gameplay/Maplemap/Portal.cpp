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
	Portal::Portal(const Animation* a, PtType t, string n, bool in, 
		vector2d<int32_t> p, int32_t tid, string tnm) {

		anim = a;
		type = t;
		name = n;
		intermap = in;
		pos = p;
		warpinfo = std::make_pair(tid, tnm);
		touched = false;
	}

	Portal::Portal() {}

	Portal::~Portal() {}

	void Portal::draw(vector2d<int32_t> viewpos, float inter) const
	{
		if (!anim || (type == HIDDEN && !touched))
			return;

		using::Graphics::DrawArgument;
		anim->draw(DrawArgument(pos + viewpos), inter);
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

	vector2d<int32_t> Portal::getposition() const
	{
		return pos;
	}

	rectangle2d<int32_t> Portal::bounds() const
	{
		return rectangle2d<int32_t>(pos, pos + vector2d<int32_t>(30, -50));
	}

	const pair<int32_t, string>* Portal::getwarpinfo() const
	{
		return &warpinfo;
	}
}
