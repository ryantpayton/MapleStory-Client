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
#include "Mapobject.h"
#include "Ptrmap.h"
#include <vector>

namespace Gameplay
{
	// Types of mapobjects. Not used by Mapobjects themselves, but by the Stage.
	enum MapobjectType
	{
		MOT_CHAR,
		MOT_NPC,
		MOT_REACTOR,
		MOT_MOB,
		MOT_DROP
	};

	using::Util::Ptrmap;
	// Base class for a collection of mapobjects of a type. 
	class Mapobjects
	{
	public:
		virtual ~Mapobjects() {}
		// Draw all mapobjects that are on the specified layer.
		// Parameters: char(layer), vector2d<int>(camera position)
		void draw(int8_t, vector2d<int32_t>) const;
		// Update all mapobjects of this type. Also updates layers eg. drawing order.
		// Parameters: Physics&(engine to use), short(delay-per-frame)
		void update(const Physics&, uint16_t);
		// Removes all mapobjects of this type.
		void clear();
	protected:
		// Adds a mapobject of this type. The parameter must be a dynamically allocated (new) Mapobject. Only used by subclasses.
		// Parameters: Mapobject*(new Mapobject)
		void add(Mapobject*);
		// Removes the mapobject with the given oid. Only used by subclasses.
		// Parameters: int(object id)
		void remove(int32_t);
		// Obtains a pointer to the mapobject with the given oid. Only used by subclasses.
		// Parameters: int(object id)
		Mapobject* get(int32_t);
		Ptrmap<int32_t, Mapobject> objects;
		map<int8_t, map<int32_t, int32_t>> layers;
	};
}

