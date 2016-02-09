//////////////////////////////////////////////////////////////////////////////
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
#include "Physics.h"

namespace Gameplay
{
	const double GRAVFORCE = 0.2f;
	const double FRICTION = 0.3f;
	const double SLOPEFACTOR = 0.1f;
	const double FLYFRICTION = 0.05f;

	Physics::Physics() {}

	Physics::~Physics() {}

	void Physics::loadfht(node src)
	{
		fht = Footholdtree(src);
	}

	void Physics::parsefht(InPacket& recv)
	{
		fht = Footholdtree(recv);
	}

	void Physics::moveobject(PhysicsObject& phobj) const
	{
		// Determine which platform the object is currently on.
		fht.updatefh(phobj);

		// Use the appropriate physics for the terrain the object is on.
		switch (phobj.type)
		{
		case PhysicsObject::NORMAL:
			movenormal(phobj);
			fht.limitmoves(phobj);
			break;
		case PhysicsObject::FLYING:
			moveflying(phobj);
			fht.limitmoves(phobj);
			break;
		case PhysicsObject::CLIMBING:
			break;
		}

		// Move the object forward.
		phobj.lastx = phobj.fx;
		phobj.lasty = phobj.fy;
		phobj.fx += phobj.hspeed;
		phobj.fy += phobj.vspeed;
	}

	void Physics::movenormal(PhysicsObject& phobj) const
	{
		phobj.vacc = phobj.vforce / phobj.mass;
		phobj.vforce = 0.0f;

		phobj.hacc = 0.0f;
		if (phobj.onground)
		{
			phobj.hacc += phobj.hforce / phobj.mass;
			phobj.hforce = 0.0f;

			if (phobj.hacc == 0.0f && phobj.hspeed < 0.1f && phobj.hspeed > -0.1f)
			{
				phobj.hspeed = 0.0f;
			}
			double slopef = phobj.fhslope;
			if (slopef > 0.5f)
				slopef = 0.5f;
			else if (slopef < -0.5f)
				slopef = -0.5f;
			phobj.hacc -= (SLOPEFACTOR * (1.0f + (slopef * -phobj.hspeed)) + FRICTION) * phobj.hspeed;
		}
		else
		{
			phobj.vacc += GRAVFORCE / phobj.mass;
		}

		phobj.hspeed += phobj.hacc;
		phobj.vspeed += phobj.vacc;
	}

	void Physics::moveflying(PhysicsObject& phobj) const
	{
		phobj.hacc = phobj.hforce / phobj.mass;
		phobj.vacc = phobj.vforce / phobj.mass;
		phobj.hforce = 0.0f;
		phobj.vforce = 0.0f;

		phobj.hacc -= FLYFRICTION * phobj.hspeed;
		phobj.vacc -= FLYFRICTION * phobj.vspeed;

		phobj.hspeed += phobj.hacc;
		phobj.vspeed += phobj.vacc;

		if (phobj.hacc == 0.0f && phobj.hspeed < 0.1f && phobj.hspeed > -0.1f)
		{
			phobj.hspeed = 0.0f;
		}
		if (phobj.vacc == 0.0f && phobj.vspeed < 0.1f && phobj.vspeed > -0.1f)
		{
			phobj.vspeed = 0.0f;
		}
	}

	vector2d<int16_t> Physics::getgroundbelow(vector2d<int16_t> position)
	{
		int16_t ground = fht.getgroundbelow(position);
		return vector2d<int16_t>(position.x(), ground - 1);
	}

	const Footholdtree& Physics::getfht() const
	{
		return fht;
	}
}
