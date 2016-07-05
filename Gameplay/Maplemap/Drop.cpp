/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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

namespace jrc
{
	Drop::Drop(int32_t id, int32_t own, Point<int16_t> start, Point<int16_t> dst, 
		int8_t type, int8_t mode, bool pldrp) : MapObject(id) {

		owner = own;
		set_position(start.x(), start.y() - 4);
		dest = dst;
		pickuptype = type;
		playerdrop = pldrp;

		angle.set(0.0f);
		opacity.set(1.0f);
		moved = 0.0f;
		looter = nullptr;

		switch (mode)
		{
		case 0:
		case 1:
			state = DROPPED;
			basey = static_cast<double>(dest.y() - 4);
			phobj.vspeed = -5.0f;
			phobj.hspeed = static_cast<double>(dest.x() - start.x()) / 48;
			break;
		case 2:
			state = FLOATING;
			basey = phobj.crnt_y();
			phobj.type = PhysicsObject::FIXATED;
			break;
		case 3:
			state = PICKEDUP;
			phobj.vspeed = -5.0f;
			break;
		}
	}

	int8_t Drop::update(const Physics& physics)
	{
		physics.move_object(phobj);

		if (state == DROPPED)
		{
			if (phobj.onground)
			{
				phobj.hspeed = 0.0;
				phobj.type = PhysicsObject::FIXATED;
				state = FLOATING;
				angle.set(0.0f);
				set_position(dest.x(), dest.y() - 4);
			}
			else
			{
				static const float SPINSTEP = 0.2f;
				angle += SPINSTEP;
			}
		}
		
		if (state == FLOATING)
		{
			phobj.y = basey + 5.0f + (cos(moved) - 1.0f) * 2.5f;
			moved = (moved < 360.0f) ? moved + 0.025f : 0.0f;
		}
		
		if (state == PICKEDUP)
		{
			static const uint16_t PICKUPTIME = 48;
			static const float OPCSTEP = 1.0f / PICKUPTIME;

			if (looter)
			{
				double hdelta = looter->x - phobj.x;
				phobj.hspeed = looter->hspeed / 2.0 + (hdelta - 16.0) / PICKUPTIME;
			}

			opacity -= OPCSTEP;
			if (opacity.last() <= OPCSTEP)
			{
				opacity.set(1.0f);

				MapObject::deactivate();
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
			break;
		case 1:
			deactivate();
			break;
		case 2:
			angle.set(0.0f);
			state = PICKEDUP;
			looter = lt;
			phobj.vspeed = -4.5f;
			phobj.type = PhysicsObject::NORMAL;
			break;
		}
	}

	Rectangle<int16_t> Drop::bounds() const
	{ 
		auto lt = get_position();
		auto rb = lt + Point<int16_t>(32, 32);
		return Rectangle<int16_t>(lt, rb); 
	}
}