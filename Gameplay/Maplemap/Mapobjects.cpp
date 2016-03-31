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
#include <vector>

namespace Gameplay
{
	void MapObjects::draw(int8_t layer, Point<int16_t> viewpos, float alpha) const
	{
		for (auto& oid : layers[layer])
		{
			auto mmo = get(oid);
			if (mmo && mmo->isactive())
			{
				mmo->draw(viewpos, alpha);
			}
		}
	}

	void MapObjects::update(const Physics& physics)
	{
		vector<int32_t> toremove;
		for (auto& obit : objects)
		{
			int32_t oid = obit.first;
			MapObject* mmo = obit.second.get();
			if (mmo)
			{
				int8_t oldlayer = mmo->getlayer();
				int8_t newlayer = mmo->update(physics);
				if (newlayer == -1)
				{
					toremove.push_back(oid);
				}
				else if (newlayer != oldlayer)
				{
					layers[oldlayer].erase(oid);
					layers[newlayer].insert(oid);
				}
			}
			else
			{
				toremove.push_back(oid);
			}
		}
		for (auto& oid : toremove)
		{
			remove(oid);
		}
	}

	void MapObjects::clear()
	{
		objects.clear();

		for (uint8_t i = 0; i < MapLayers::NUM_LAYERS; i++)
		{
			layers[i].clear();
		}
	}

	void MapObjects::add(MapObject* toadd)
	{
		if (toadd)
		{
			int32_t oid = toadd->getoid();
			int8_t layer = toadd->getlayer();
			objects[oid] = unique_ptr<MapObject>(toadd);
			layers[layer].insert(oid);
		}
	}

	void MapObjects::remove(int32_t oid)
	{
		auto iter = objects.find(oid);
		if (iter != objects.end() && iter->second)
		{
			int8_t layer = iter->second->getlayer();
			objects.erase(iter);

			layers[layer].erase(oid);
		}
	}

	Optional<MapObject> MapObjects::get(int32_t oid)
	{
		auto iter = objects.find(oid);
		return iter != objects.end() ? iter->second.get() : nullptr;
	}

	Optional<const MapObject> MapObjects::get(int32_t oid) const
	{
		auto iter = objects.find(oid);
		return iter != objects.end() ? iter->second.get() : nullptr;
	}
}
