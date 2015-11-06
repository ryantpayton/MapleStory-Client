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
	Portal::Portal(PortalType t, string n, int32_t tid, bool in, string tnm, vector2d<int32_t> pos)
	{
		type = t;
		name = n;
		intermap = in;
		warpinfo = std::make_pair(tid, tnm);
		position = pos;
	}

	const string& Portal::getname() const
	{
		return name;
	}

	PortalType Portal::gettype() const
	{
		return type;
	}

	vector2d<int32_t> Portal::getposition() const
	{
		return position;
	}

	rectangle2d<int32_t> Portal::bounds() const
	{
		return rectangle2d<int32_t>(position, position + vector2d<int32_t>(30, -50));
	}

	const pair<int32_t, string>* Portal::getwarpinfo() const
	{
		return &warpinfo;
	}
}
