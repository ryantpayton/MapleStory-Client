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
#include "MapLayer.h"
#include "nlnx\nx.hpp"

namespace Gameplay
{
	MapLayer::MapLayer(node src)
	{
		string tileset = src["info"]["tS"] + ".img";
		node tilesrc = src["tile"];
		for (node tilenode = tilesrc.begin(); tilenode != tilesrc.end(); ++tilenode)
		{
			Tile tile = Tile(tilenode, tileset);
			tiles[tile.getz()].push_back(tile);
		}

		node objsrc = src["obj"];
		for (node objnode = objsrc.begin(); objnode != objsrc.end(); ++objnode)
		{
			Obj obj = Obj(objnode);
			objs[obj.getz()].push_back(obj);
		}
	}

	MapLayer::MapLayer() {}

	MapLayer::~MapLayer() {}

	void MapLayer::update()
	{
		for (map<uint8_t, vector<Obj>>::iterator lyit = objs.begin(); lyit != objs.end(); ++lyit)
		{
			for (vector<Obj>::iterator obit = lyit->second.begin(); obit != lyit->second.end(); ++obit)
			{
				obit->update();
			}
		}
	}

	void MapLayer::draw(vector2d<int32_t> viewpos, float inter) const
	{
		for (map<uint8_t, vector<Obj>>::const_iterator lyit = objs.begin(); lyit != objs.end(); ++lyit)
		{
			for (vector<Obj>::const_iterator obit = lyit->second.begin(); obit != lyit->second.end(); ++obit)
			{
				obit->draw(viewpos, inter);
			}
		}

		for (map<uint8_t, vector<Tile>>::const_iterator lyit = tiles.begin(); lyit != tiles.end(); ++lyit)
		{
			for (vector<Tile>::const_iterator tlit = lyit->second.begin(); tlit != lyit->second.end(); ++tlit)
			{
				tlit->draw(viewpos);
			}
		}
	}
}
