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
#include "Layer.h"

namespace Gameplay
{
	const uint8_t NUM_LAYERS = 8;

	// Represents all obj and tile layers on a map. Responsible for loading, drawing and updating them.
	class Maplayers
	{
	public:
		// Empty constructor.
		Maplayers();
		// Calls clear, destroying all objs and tilesets in cache.
		~Maplayers();
		// Loads all layers from the specified node of map data.
		void load(node);
		// Clears the cache. To be called when loading a new map, but not when changing channel.
		void clear();
		// Draw the specified layer at the specified position.
		// Parameters: uint8_t(layer to draw), vector2d(camera position)
		void draw(uint8_t, vector2d<int32_t>) const;
		// Updates the animation of all objs. All objs of the same kind share the same animation.
		// Parameters: uint16_t(delay-per-frame)
		void update(uint16_t);
	private:
		LayerData layerdata;
		map<uint8_t, Layer> layers;
	};
}

