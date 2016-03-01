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
#include "MapLayer.h"
#include "MapObject.h"
#include "Util\Optional.h"
#include <memory>
#include <unordered_map>
#include <map>

namespace Gameplay
{
	using std::unique_ptr;
	using std::unordered_map;
	using std::map;

	// Collection of mapobjects of a type. 
	class MapObjects
	{
	public:
		virtual ~MapObjects() {}

		// Draw all mapobjects that are on the specified layer.
		void draw(int8_t layer, const Camera& camera, float inter) const;
		// Update all mapobjects of this type. Also updates layers eg. drawing order.
		virtual void update(const Physics& physics);
		// Removes all mapobjects of this type.
		void clear();
		// Adds a mapobject of this type.
		void add(MapObject* mapobject);
		// Removes the mapobject with the given oid.
		void remove(int32_t oid);

	protected:
		// Obtains a pointer to the mapobject with the given oid.
		Optional<MapObject> get(int32_t oid);

		unordered_map<int32_t, unique_ptr<MapObject>> objects;
		map<int32_t, int32_t> layers[MapLayer::NUM_LAYERS];
	};
}

