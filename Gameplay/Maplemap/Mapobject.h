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
#include "Gameplay\Physics\Physics.h"
#include "Gameplay\Camera.h"

namespace Gameplay
{
	// Base for objects on a map, eg. mobs, npcs, characters etc.
	class MapObject
	{
	public:
		virtual ~MapObject(){}

		// Draws the object.
		virtual void draw(const Camera& camera, float inter) const = 0;

		// Updates the object and returns the updated layer.
		virtual int8_t update(const Physics& physics);
		virtual void setactive(bool active);
		virtual bool isactive() const;

		// Changes the objects position.
		void setposition(int16_t x, int16_t y);

		// Obtains the layer used to determine the drawing order on the map.
		virtual int8_t getlayer() const;
		// Returns the object id unique to every object on one map.
		int32_t getoid() const;
		// Returns the current position.
		vector2d<int16_t> getposition() const;
		vector2d<double> getrealposition() const;
		PhysicsObject& getphobj();

	protected:
		PhysicsObject phobj;
		int32_t oid;
		bool active;
	};
}