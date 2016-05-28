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
#include "MapLayers.h"
#include "nlnx\nx.hpp"

namespace jrc
{
	Layer::Layer(nl::node src)
	{
		auto tileset = src["info"]["tS"] + ".img";
		for (auto tilenode : src["tile"])
		{
			Tile tile(tilenode, tileset);
			tiles[tile.getz()]
				.push_back(tile);
		}

		for (auto objnode : src["obj"])
		{
			Obj obj(objnode);
			objs[obj.getz()]
				.push_back(obj);
		}
	}

	Layer::Layer() {}

	void Layer::update()
	{
		for (auto& lyit : objs)
		{
			for (auto& obit : lyit.second)
			{
				obit.update();
			}
		}
	}

	void Layer::draw(Point<int16_t> viewpos, float alpha) const
	{
		for (auto& lyit : objs)
		{
			for (auto& obit : lyit.second)
			{
				obit.draw(viewpos, alpha);
			}
		}

		for (auto& lyit : tiles)
		{
			for (auto& tlit : lyit.second)
			{
				tlit.draw(viewpos);
			}
		}
	}

	MapLayers::MapLayers(nl::node src)
	{
		for (size_t i = 0; i < NUM_LAYERS; i++)
		{
			layers[i] = src[std::to_string(i)];
		}
	}

	MapLayers::MapLayers() {}

	void MapLayers::draw(uint8_t layer, Point<int16_t> viewpos, float alpha) const
	{
		layers[layer]
			.draw(viewpos, alpha);
	}

	void MapLayers::update()
	{
		for (size_t i = 0; i < NUM_LAYERS; i++)
		{
			layers[i].update();
		}
	}
}
