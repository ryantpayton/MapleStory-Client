//////////////////////////////////////////////////////////////////////////////
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
#include "Mapobjects.h"

namespace Gameplay
{
	void MapObjects::draw(int8_t layer, vector2d<int16_t> pos, float inter) const
	{
		if (layers.count(layer))
		{
			for (auto& lyit : layers.at(layer))
			{
				int32_t oid = lyit.second;
				if (objects.count(oid))
				{
					objects.at(oid)->draw(pos, inter);
				}
			}
		}
	}

	void MapObjects::update(const Physics& fht)
	{
		using std::vector;
		vector<int32_t> toremove;

		for (auto& obit : objects)
		{
			int8_t oldlayer = obit.second->getlayer();
			int8_t newlayer = obit.second->update(fht);
			if (newlayer != oldlayer)
			{
				int32_t oid = obit.second->getoid();
				layers[oldlayer].erase(oid);
				if (newlayer == -1)
				{
					toremove.push_back(oid);
				}
				else
				{
					layers[newlayer][oid] = oid;
				}
			}
		}
		for (auto& rmit : toremove)
		{
			remove(rmit);
		}
	}

	void MapObjects::clear()
	{
		objects.clear();
		layers.clear();
	}

	void MapObjects::add(MapObject* toadd)
	{
		if (toadd)
		{
			int32_t oid = toadd->getoid();
			int8_t layer = toadd->getlayer();
			objects[oid] = unique_ptr<MapObject>(toadd);
			layers[layer][oid] = oid;
		}
	}

	void MapObjects::remove(int32_t oid)
	{
		if (objects.count(oid))
		{
			int8_t layer = objects[oid]->getlayer();
			objects[oid].release();
			objects.erase(oid);
			layers[layer].erase(oid);
		}
	}

	MapObject* MapObjects::get(int32_t oid)
	{
		if (objects.count(oid))
			return objects[oid].get();
		else
			return nullptr;

	}
}
