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
#include "PhysicsEngineNormal.h"
#include "Footholdtree.h"
#include "PhysicsObject.h"

namespace Gameplay
{
	// Class that uses physics engines and the collection of platforms to determine object movement.
	class Physics
	{
	public:
		Physics();
		~Physics();
		// Load a footholdtree (collection of platforms) from a node of game data.
		// Parameters: node('foothold' node of map data)
		void loadfht(node);
		// Move the specified object over the specified game-time.
		// Parameters: PhysicsObject&(object to move), short(delay-per-frame)
		void moveobject(PhysicsObject&, uint16_t) const;
		// Return a reference to the collection of platforms.
		const Footholdtree& getfht() const;
	private:
		PhysicsEngineNormal phenormal;
		Footholdtree fht;
	};
}

