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
#include "Util\AlgorithmExt.h"
#include <vector>

namespace Gameplay
{
	void MapObjects::draw(int8_t layer, Point<int16_t> viewpos, float inter) const
	{
		for (auto& oid : layers[layer])
		{
			Optional<const MapObject> mmo = get(oid);
			if (mmo && mmo->isactive())
			{
				mmo->draw(viewpos, inter);
			}
		}
	}

	void MapObjects::update(const Physics& physics)
	{
		Algorithm::remove_all<int32_t, unique_ptr<MapObject>>(objects, 
			[&](unique_ptr<MapObject>& obit){
			MapObject* mmo = obit.get();
			if (mmo)
			{
				int8_t oldlayer = mmo->getlayer();
				int8_t newlayer = mmo->update(physics);
				if (newlayer == -1)
					return true;

				if (newlayer != oldlayer)
				{
					int32_t oid = mmo->getoid();
					layers[oldlayer].erase(oid);
					layers[newlayer].insert(oid);
				}
				return false;
			}
			else
			{
				return true;
			}
		});
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
		Optional<MapObject> mmo = get(oid);
		if (mmo)
		{
			int8_t layer = mmo->getlayer();
			layers[layer].erase(oid);
			objects.erase(oid);
		}
	}

	Optional<MapObject> MapObjects::get(int32_t oid)
	{
		return objects.count(oid) ? objects.at(oid).get() : Optional<MapObject>();
	}

	Optional<const MapObject> MapObjects::get(int32_t oid) const
	{
		return objects.count(oid) ? objects.at(oid).get() : Optional<const MapObject>();
	}
}
