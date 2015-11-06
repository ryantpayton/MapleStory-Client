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
#include "nx.hpp"

namespace Gameplay
{
	Layer::Layer(node src, LayerData& layerdata)
	{
		const Tileset& tileset = layerdata.gettileset(src["info"]["tS"]);
		node tilesrc = src["tile"];
		for (node tilenode = tilesrc.begin(); tilenode != tilesrc.end(); ++tilenode)
		{
			const TileData& tdata = tileset.gettile(tilenode["u"], tilenode["no"]);
			if (tdata.isloaded())
			{
				tiles[tdata.getz()].push_back(Tile(tdata, vector2d<int32_t>(tilenode["x"], tilenode["y"])));
			}
		}

		node objsrc = src["obj"];
		for (node objnode = objsrc.begin(); objnode != objsrc.end(); ++objnode)
		{
			int32_t z = objnode["z"];
			node asrc = nl::nx::map["Obj"][objnode["oS"] + ".img"][objnode["l0"]][objnode["l1"]][objnode["l2"]];
			vector2d<int32_t> pos = vector2d<int32_t>(objnode["x"], objnode["y"]);
			bool flip = objnode["f"].get_bool();
			objs[z].push_back(Obj(layerdata.getobj(asrc), pos, flip));
		}
	}

	Layer::Layer() {}

	Layer::~Layer() {}

	void Layer::draw(vector2d<int32_t> viewpos) const
	{
		for (map<uint8_t, vector<Obj>>::const_iterator lyit = objs.begin(); lyit != objs.end(); ++lyit)
		{
			for (vector<Obj>::const_iterator obit = lyit->second.begin(); obit != lyit->second.end(); ++obit)
			{
				obit->draw(viewpos);
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
