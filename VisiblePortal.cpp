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
#include "VisiblePortal.h"

namespace Gameplay
{
	VisiblePortal::VisiblePortal(Animation& a, PortalType t, string n, int tid, bool in, string tnm, vector2d<int> pos) : anim(a), Portal(t, n, tid, in, tnm, pos) 
	{
		touched = false;
	}

	void VisiblePortal::draw(vector2d<int> viewpos)
	{
		if (type != PT_HIDDEN || touched)
		{
			anim.draw(PosArgument(position + viewpos));
		}
	}

	void VisiblePortal::settouch(bool t)
	{
		touched = t;
	}
}
