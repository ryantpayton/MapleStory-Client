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
#pragma once
#include "Physics.h"

namespace Gameplay
{
	// Default delay.
	const uint16_t BASEDPF = 16;

	Physics::Physics() {}

	Physics::~Physics() {}

	void Physics::loadfht(node src)
	{
		fht = Footholdtree(src);
	}

	void Physics::moveobject(PhysicsObject& phobj, uint16_t dpf) const
	{
		// Determine which platform the object is currently on.
		fht.updatefh(phobj);

		// Calculate a multiplier from the default delay.
		float dpfmult = static_cast<float>(dpf) / BASEDPF;
		// Use the appropriate physics engine for the terrain the object is on.
		switch (phobj.terrain)
		{
		case PHT_NORMAL:
			phenormal.applyforces(phobj, dpfmult);
			break;
		}

		// Check for collisions and restrict movement.
		fht.limitmoves(phobj, dpfmult);

		// Move the object forward. s = 1/2 * a * dt * dt;
		phobj.fx += phobj.hspeed * dpfmult;
		phobj.fy += phobj.vspeed * dpfmult;
	}

	const Footholdtree& Physics::getfht() const
	{
		return fht;
	}
}
