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
#include "Footholdtree.h"

namespace Gameplay
{
	// Class that uses physics engines and the collection of platforms to determine object movement.
	class Physics
	{
	public:
		Physics();
		~Physics();

		// Load the physics for a map.
		void load(node source);
		// Parse a footholdtree (collection of platforms) from a packet.
		void parsefht(InPacket& recv);
		// Move the specified object over the specified game-time.
		void moveobject(PhysicsObject& tomove) const;
		// Determine the point on the ground below the specified position.
		Point<int16_t> getgroundbelow(Point<int16_t> position) const;
		// Return a reference to the collection of platforms.
		const Footholdtree& getfht() const;

	private:
		void movenormal(PhysicsObject&) const;
		void moveflying(PhysicsObject&) const;
		void moveswimming(PhysicsObject&) const;

		Footholdtree fht;
	};
}

