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
#include "Tile.h"
#include "Ptrmap.h"

namespace Gameplay
{
	using::std::string;
	using::std::map;
	using::Util::Ptrmap;
	// A tileset loaded from a .img of game data in Map.nx/Tile
	class Tileset
	{
	public:
		// Construct the tileset by loading all tiles from the specified node.
		Tileset(node);
		// Default empty tileset.
		Tileset();
		// Empty destructor.
		~Tileset();
		// Return a tile from the specified node with the specified number.
		// Parmeters: string(u-node of a tile), int32_t(no-node of a tile)
		const TileData& gettile(string, int32_t) const;
	private:
		map<string, Ptrmap<int32_t, TileData>> tiles;
		TileData nulltile;
	};
}

