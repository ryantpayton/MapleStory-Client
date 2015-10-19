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
#include "Portal.h"

namespace Gameplay
{
	Portal::Portal(PortalType t, string n, int tid, bool in, string tnm, vector2d<int> pos)
	{
		type = t;
		name = n;
		targetid = tid;
		intermap = in;
		targetpname = tnm;
		position = pos;
	}

	string Portal::getname()
	{
		return name;
	}

	PortalType Portal::gettype()
	{
		return type;
	}

	vector2d<int> Portal::getposition()
	{
		return position;
	}

	rectangle2d<int> Portal::bounds()
	{
		return rectangle2d<int>(position, position + vector2d<int>(30, -50));
	}

	pair<int, string> Portal::getwarpinfo()
	{
		return make_pair(targetid, intermap ? targetpname : name);
	}
}
