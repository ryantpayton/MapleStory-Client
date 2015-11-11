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
#include "Gameplay\Maplemap\Mapobject.h"
#include <memory>
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

	using::std::unique_ptr;

	// Collection of mapobjects of a type. 
	class Mapobjects
	{
	public:
		virtual ~Mapobjects() {}
		// Draw all mapobjects that are on the specified layer.
		void draw(int8_t, vector2d<int32_t>, float) const;
		// Update all mapobjects of this type. Also updates layers eg. drawing order.
		// Parameters: Physics&(engine to use)
		void update(const Physics&);
		// Removes all mapobjects of this type.
		void clear();
		// Adds a mapobject of this type.
		// Parameters: Mapobject*(new Mapobject)
		void add(Mapobject*);
		// Removes the mapobject with the given oid.
		// Parameters: int(object id)
		void remove(int32_t);
	protected:
		// Obtains a pointer to the mapobject with the given oid.
		// Parameters: int(object id)
		Mapobject* get(int32_t);
		map<int32_t, unique_ptr<Mapobject>> objects;
		map<int8_t, map<int32_t, int32_t>> layers;
	};
}

