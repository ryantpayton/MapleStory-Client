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
#include "Foothold.h"
#include "PhysicsObject.h"
#include <map>
#include <vector>

namespace Gameplay
{
	using::std::map;
	using::std::vector;

	// The collection of platforms in a maple-map. Used for collision-detection.
	class Footholdtree
	{
	public:
		// Parameters: node('foothold' node of map data)
		Footholdtree(node);
		Footholdtree();
		~Footholdtree();
		// Takes an accelerated PhysicsObject and limits its movement based on the platforms in this tree.
		// Parameters: PhysicsObject&(object to use), float(framerate-multiplier)
		void limitmoves(PhysicsObject&, float) const;
		// Updates a PhysicsObject's fhid based on it's position.
		// Parameters: PhysicsObject&(object to use)
		void updatefh(PhysicsObject&) const;
		// Returns the leftmost and rightmost platform positions of the map.
		vector2d<int32_t> getwalls() const;
		// Returns the topmost and bottommost platform positions of the map.
		vector2d<int32_t> getborders() const;
	private:
		const Foothold& getfh(uint16_t) const;
		uint16_t getnext(uint16_t, bool, float, float) const;
		uint16_t getbelow(float, float) const;
		int32_t getlwall(vector2d<int32_t>) const;
		int32_t getrwall(vector2d<int32_t>) const;
		float getwall(uint16_t, bool, vector2d<int32_t>) const;

		map<uint16_t, Foothold> footholds;
		Foothold nullfh;
		vector<uint16_t> edgesl;
		vector<uint16_t> edgesr;
		vector2d<int32_t> walls;
		vector2d<int32_t> borders;
	};
}

