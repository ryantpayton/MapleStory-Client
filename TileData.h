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
#include "Texture.h"

namespace Gameplay
{
	using::std::int32_t;
	using::nl::node;
	using::Util::vector2d;
	using::Graphics::Texture;
	// Tile loaded from Map.nx/Tile.img. Contains a Texture and a z (depth) value.
	class TileData
	{
	public:
		// Load the tile from a node of game data.
		TileData(node);
		// Default tile, can be used if a tile is requested that does not exist.
		TileData();
		// Delete the texture object at destruction.
		~TileData();
		// Draw the tile at the specified position.
		void draw(vector2d<int32_t>) const;
		// Return if the tile was loaded. False for default tile.
		bool isloaded() const;
		// Return the z (depth) value associated with this tile.
		int32_t getz() const;
	private:
		Texture* tiletexture;
		int32_t z;
	};
}

