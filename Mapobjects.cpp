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
	void Mapobjects::draw(int8_t layer, vector2d<int32_t> viewpos) const
	{
		if (layers.count(layer))
		{
			for (map<int32_t, int32_t>::const_iterator lyit = layers.at(layer).begin(); lyit != layers.at(layer).end(); ++lyit)
			{
				const Mapobject* obj = objects.get(lyit->second);
				if (obj)
				{
					obj->draw(viewpos);
				}
			}
		}
	}

	void Mapobjects::update(const Physics& fht, uint16_t dpf)
	{
		vector<int32_t> toremove;
		for (map<int32_t, Mapobject*>::iterator obit = objects.getbegin(); obit != objects.getend(); ++obit)
		{
			Mapobject* obj = obit->second;
			if (obj)
			{
				int8_t oldlayer = obj->getlayer();
				int8_t newlayer = obj->update(fht, dpf);
				if (newlayer != oldlayer)
				{
					if (newlayer == -1)
					{
						toremove.push_back(obit->first);
					}
					else
					{
						int32_t oid = obj->getoid();
						layers[oldlayer].erase(oid);
						layers[newlayer][oid] = oid;
					}
				}
			}
		}
		for (vector<int32_t>::iterator rmit = toremove.begin(); rmit != toremove.end(); ++rmit)
		{
			remove(*rmit);
		}
	}

	void Mapobjects::clear()
	{
		objects.clear();
		layers.clear();
	}

	void Mapobjects::add(Mapobject* toadd)
	{
		if (toadd)
		{
			int32_t oid = toadd->getoid();
			int8_t layer = toadd->getlayer();
			objects.add(oid, toadd);
			layers[layer][oid] = oid;
		}
	}

	void Mapobjects::remove(int32_t oid)
	{
		Mapobject* obj = objects.get(oid);
		if (obj)
		{
			int8_t layer = obj->getlayer();
			objects.remove(oid);
			layers[layer].erase(oid);
		}
	}

	Mapobject* Mapobjects::get(int32_t oid)
	{
		return objects.get(oid);
	}
}
