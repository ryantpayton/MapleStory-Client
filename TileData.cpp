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
#include "Tile.h"

namespace Gameplay
{
	TileData::TileData(node src)
	{
		tiletexture = new Texture(src);
		z = (src["z"].data_type() == node::type::integer) ? src["z"] : src["zM"];
	}

	TileData::TileData()
	{
		tiletexture = nullptr;
		z = 0;
	}

	TileData::~TileData()
	{
		if (tiletexture) delete tiletexture;
	}

	void TileData::draw(vector2d<int32_t> pos) const
	{
		if (tiletexture)
		{
			using::Graphics::PosArgument;
			tiletexture->draw(PosArgument(pos));
		}
	}

	bool TileData::isloaded() const
	{
		return tiletexture != nullptr;
	}

	int32_t TileData::getz() const
	{
		return z;
	}
}
