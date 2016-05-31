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

namespace jrc
{
	void MapObjects::draw(int8_t layer, double viewx, double viewy, float alpha) const
	{
		for (auto& oid : layers[layer])
		{
			auto mmo = get(oid);
			if (mmo && mmo->isactive())
			{
				mmo->draw(viewx, viewy, alpha);
			}
		}
	}

	void MapObjects::update(const Physics& physics)
	{
		for (auto iter = objects.begin(); iter != objects.end();)
		{
			bool remove_mob = false;
			int32_t oid = iter->first;
			MapObject* mmo = iter->second.get();
			if (mmo)
			{
				int8_t oldlayer = mmo->getlayer();
				int8_t newlayer = mmo->update(physics);
				if (newlayer == -1)
				{
					remove_mob = true;
				}
				else if (newlayer != oldlayer)
				{
					layers[oldlayer].erase(oid);
					layers[newlayer].insert(oid);
				}
			}
			else
			{
				remove_mob = true;
			}

			if (remove_mob)
			{
				iter = objects.erase(iter);
			}
			else
			{
				iter++;
			}
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

	void MapObjects::add(std::unique_ptr<MapObject> toadd)
	{
		if (toadd)
		{
			int32_t oid = toadd->getoid();
			int8_t layer = toadd->getlayer();
			objects[oid] = std::move(toadd);
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

	MapObjects::underlying_t::iterator MapObjects::begin()
	{
		return objects.begin();
	}

	MapObjects::underlying_t::iterator MapObjects::end()
	{
		return objects.end();
	}

	MapObjects::underlying_t::const_iterator MapObjects::begin() const
	{
		return objects.begin();
	}

	MapObjects::underlying_t::const_iterator MapObjects::end() const
	{
		return objects.end();
	}
}
