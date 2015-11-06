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
#include "TileData.h"

namespace Gameplay
{
	using::std::int32_t;
	using::Util::vector2d;
	// Represents a concrete tile on a map. Shares tile data with other tiles.
	class Tile
	{
	public:
		// Construct the tile from constant tile data and a position on the map.
		// Parameters: TileData&(data shared between tiles), vector2d(position)
		Tile(const TileData&, vector2d<int32_t>);
		~Tile();
		// Draw the tile.
		// Parameters: vector2d(position of camera)
		void draw(vector2d<int32_t>) const;
	private:
		const TileData& tdata;
		vector2d<int32_t> position;
	};
}

