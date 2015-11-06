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
#pragma once
#include "PhysicsEngineNormal.h"
#include <cmath>

namespace Gameplay
{
	// Constants for physics calculations.
	const float GRAVFORCE = 1.0f;
	const float FRICTION = 0.5f;

	PhysicsEngineNormal::PhysicsEngineNormal() {}

	PhysicsEngineNormal::~PhysicsEngineNormal() {}

	void PhysicsEngineNormal::applyforces(PhysicsObject& phobj, float dpfmult) const
	{
		// Initialise acceleration with self-applied forces.
		phobj.hacc = phobj.hforce / phobj.mass;
		phobj.vacc = phobj.vforce / phobj.mass;
		phobj.hforce = 0.0f;
		phobj.vforce = 0.0f;

		if (phobj.onground)
		{
			// Make sure speed doesn't diverge.
			if (phobj.hacc == 0.0f && phobj.hspeed < 0.1f && phobj.hspeed > -0.1f)
			{
				phobj.hspeed = 0.0f;
			}
			// Apply slope acceleration.
			phobj.hacc += phobj.fhslope * phobj.hspeed;
			// Apply friction.
			phobj.hacc -= FRICTION * phobj.hspeed;
		}
		else
		{
			// Apply gravity.
			phobj.vacc += GRAVFORCE / phobj.mass;
		}
		// Add accelerations to speed.
		phobj.hspeed += phobj.hacc * dpfmult;
		phobj.vspeed += phobj.vacc * dpfmult;
	}
}
