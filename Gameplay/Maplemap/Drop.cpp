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
#include "Drop.h"

namespace Gameplay
{
	Drop::Drop(int32_t id, int32_t own, vector2d<int16_t> start,
		vector2d<int16_t> dst, int8_t type, int8_t mode, bool pldrp) {

		oid = id;
		owner = own;
		setposition(start.x(), start.y() - 4);
		dest = dst;
		pickuptype = type;
		playerdrop = pldrp;

		opacity = 1.0f;
		moved = 0.0f;
		looter = nullptr;
		active = true;

		switch (mode)
		{
		case 0:
		case 1:
			state = DROPPED;
			basey = static_cast<double>(dest.y() - 4);
			phobj.vforce = -5.0f;
			phobj.hspeed = static_cast<double>(dest.x() - start.x()) / 40.0;
			break;
		case 2:
			state = FLOATING;
			basey = phobj.fy;
			phobj.type = PhysicsObject::FLYING;
			break;
		case 3:
			state = PICKEDUP;
			phobj.vforce = -5.0f;
			break;
		}
	}

	int8_t Drop::update(const Physics& physics)
	{
		physics.moveobject(phobj);

		if (state == DROPPED && phobj.onground)
		{
			setposition(dest.x(), dest.y() - 4);
			phobj.type = PhysicsObject::FLYING;
			state = FLOATING;
		}
		
		if (state == FLOATING)
		{
			phobj.fy = basey + 5.0f + (cos(moved) - 1.0f) * 2.5f;
			moved = (moved < 360.0f) ? moved + 0.025f : 0.0f;
			return 7;
		}
		
		if (state == PICKEDUP)
		{
			static const uint16_t PICKUPTIME = 48;

			if (looter)
				phobj.hspeed = looter->hspeed / 2.0 + (looter->fx - phobj.fx - 16.0) / PICKUPTIME;

			opacity -= 1.0f / PICKUPTIME;
			if (opacity <= 1.0f / PICKUPTIME)
			{
				opacity = 0.0f;
				active = false;
				return -1;
			}
		}

		return phobj.fhlayer;
	}

	void Drop::expire(int8_t type, const PhysicsObject* lt)
	{
		switch (type)
		{
		case 0:
			state = PICKEDUP;
			phobj.type = PhysicsObject::NORMAL;
			break;
		case 1:
			active = false;
			break;
		case 2:
			state = PICKEDUP;
			looter = lt;
			phobj.vforce = -4.5f;
			phobj.type = PhysicsObject::NORMAL;
			break;
		}
	}

	rectangle2d<int16_t> Drop::bounds()
	{ 
		return rectangle2d<int16_t>(
			getposition(), 
			getposition() + vector2d<int16_t>(32, 32)
			); 
	}
}