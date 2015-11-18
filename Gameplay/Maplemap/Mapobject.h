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

namespace Gameplay
{
	// Interface for objects on a map, eg. mobs, npcs, characters etc.
	class MapObject
	{
	public:
		virtual ~MapObject(){}
		// Updates the object and returns the updated layer.
		virtual int8_t update(const Physics&) = 0;
		// Draws the object.
		virtual void draw(vector2d<int16_t>, float) const = 0;
		// Changes the objects position.
		virtual void setposition(int16_t, int16_t) = 0;
		// Obtains the layer used to determine the drawing order on the map.
		virtual int8_t getlayer() const = 0;
		// Returns the object id unique to every object on one map.
		virtual int32_t getoid() const = 0;
		// Returns the current position.
		virtual vector2d<int16_t> getposition() const = 0;
	};
}