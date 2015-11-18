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
#include <unordered_map>

namespace Gameplay
{
	using std::unordered_map;
	using std::unordered_multimap;

	// The collection of platforms in a maple-map. Used for collision-detection.
	class Footholdtree
	{
	public:
		Footholdtree(node source);
		Footholdtree();
		~Footholdtree();

		// Takes an accelerated PhysicsObject and limits its movement based on the platforms in this tree.
		void limitmoves(PhysicsObject& touse) const;
		// Updates a PhysicsObject's fhid based on it's position.
		void updatefh(PhysicsObject& touse) const;

		// Returns the leftmost and rightmost platform positions of the map.
		vector2d<int16_t> getwalls() const;
		// Returns the topmost and bottommost platform positions of the map.
		vector2d<int16_t> getborders() const;

	private:
		uint16_t getbelow(float fx, float fy) const;
		float getwall(uint16_t fhid, bool left, vector2d<int16_t> vertical) const;
		const Foothold& getfh(uint16_t fhid) const;

		unordered_map<uint16_t, Foothold> footholds;
		unordered_multimap<int16_t, uint16_t> footholdsbyx;

		Foothold nullfh;
		vector2d<int16_t> walls;
		vector2d<int16_t> borders;
	};
}

