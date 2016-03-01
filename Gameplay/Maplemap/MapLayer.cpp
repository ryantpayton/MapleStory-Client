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
		for (node tilenode : src["tile"])
		{
			Tile tile = Tile(tilenode, tileset);
			tiles[tile.getz()].push_back(tile);
		}

		for (node objnode : src["obj"])
		{
			Obj obj = Obj(objnode);
			objs[obj.getz()].push_back(obj);
		}
	}

	MapLayer::MapLayer(InPacket& recv)
	{
		string tileset = recv.readascii();
		uint16_t numtiles = recv.readshort();
		for (uint16_t i = 0; i < numtiles; i++)
		{
			Tile tile = Tile(recv, tileset);
			tiles[tile.getz()].push_back(tile);
		}

		uint16_t numobjs = recv.readshort();
		for (uint16_t i = 0; i < numobjs; i++)
		{
			Obj obj = Obj(recv);
			objs[obj.getz()].push_back(obj);
		}
	}

	MapLayer::MapLayer() {}

	MapLayer::~MapLayer() {}

	void MapLayer::update()
	{
		for (auto& lyit : objs)
		{
			for (auto& obit : lyit.second)
			{
				obit.update();
			}
		}
	}

	void MapLayer::draw(Point<int16_t> viewpos, float inter) const
	{
		for (auto& lyit : objs)
		{
			for (auto& obit : lyit.second)
			{
				obit.draw(viewpos, inter);
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
}
