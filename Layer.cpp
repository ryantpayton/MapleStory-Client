/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "nx.hpp"

namespace Gameplay
{
	Layer::Layer(node src, Layercache& cache)
	{
		node tilesrc = src["tile"];
		node objsrc = src["obj"];
		Tileset* tileset = cache.gettileset(src["info"]["tS"]);
		for (node tilenode = tilesrc.begin(); tilenode != tilesrc.end(); ++tilenode)
		{
			TileData* tdata = tileset->gettile(tilenode["u"], tilenode["no"]);
			vector2d<int> pos = vector2d<int>(tilenode["x"], tilenode["y"]);
			tiles[tdata->getz()].push_back(Tile(tdata, pos));
		}
		for (node objnode = objsrc.begin(); objnode != objsrc.end(); ++objnode)
		{
			int z = objnode["z"];
			node asrc = nx::map["Obj"][objnode["oS"] + ".img"][objnode["l0"]][objnode["l1"]][objnode["l2"]];
			vector2d<int> pos = vector2d<int>(objnode["x"], objnode["y"]);
			bool flip = objnode["f"].get_bool();
			objs[z].push_back(Obj(cache.getobj(asrc), pos, flip));
		}
	}

	void Layer::draw(vector2d<int> viewpos)
	{
		for (map<char, vector<Obj>>::iterator lyit = objs.begin(); lyit != objs.end(); ++lyit)
		{
			for (vector<Obj>::iterator obit = lyit->second.begin(); obit != lyit->second.end(); ++obit)
			{
				obit->draw(viewpos);
			}
		}

		for (map<char, vector<Tile>>::iterator lyit = tiles.begin(); lyit != tiles.end(); ++lyit)
		{
			for (vector<Tile>::iterator tlit = lyit->second.begin(); tlit != lyit->second.end(); ++tlit)
			{
				tlit->draw(viewpos);
			}
		}
	}
}
