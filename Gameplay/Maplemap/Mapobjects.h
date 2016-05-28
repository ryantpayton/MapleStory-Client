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
#include "MapLayers.h"
#include "MapObject.h"

#include "..\..\Util\Optional.h"

#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace jrc
{
	// Collection of mapobjects of a type. 
	class MapObjects
	{
	public:
		virtual ~MapObjects() {}

		// Draw all mapobjects that are on the specified layer.
		virtual void draw(int8_t layer, double viewx, double viewy, float alpha) const;
		// Update all mapobjects of this type. Also updates layers eg. drawing order.
		virtual void update(const Physics& physics);

		// Removes all mapobjects of this type.
		void clear();

	protected:
		// Adds a mapobject of this type.
		void add(MapObject* mapobject);
		// Removes the mapobject with the given oid.
		void remove(int32_t oid);
		// Obtains a pointer to the mapobject with the given oid.
		Optional<MapObject> get(int32_t oid);
		// Obtains a const pointer to the mapobject with the given oid.
		Optional<const MapObject> get(int32_t oid) const;

		std::unordered_map<int32_t, std::unique_ptr<MapObject>> objects;
		std::unordered_set<int32_t> layers[MapLayers::NUM_LAYERS];
	};
}

