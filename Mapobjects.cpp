//////////////////////////////////////////////////////////////////////////////
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
#include "Mapobjects.h"

namespace Gameplay
{
	void Mapobjects::draw(char layer, vector2d<int> viewpos)
	{
		for (map<int, int>::iterator lyit = layers[layer].begin(); lyit != layers[layer].end(); ++lyit)
		{
			Mapobject* obj = objects.get(lyit->second);
			if (obj != 0)
			{
				obj->draw(viewpos);
			}
		}
	}

	void Mapobjects::update(short dpf)
	{
		vector<int> toremove;
		for (map<int, Mapobject*>::iterator obit = objects.getbegin(); obit != objects.getend(); ++obit)
		{
			Mapobject* obj = obit->second;
			if (obj != 0)
			{
				char oldlayer = obj->getlayer();
				char newlayer = obj->update(dpf);
				if (newlayer != oldlayer)
				{
					if (newlayer == -1)
					{
						toremove.push_back(obit->first);
					}
					else
					{
						int oid = obj->getoid();
						layers[oldlayer].erase(oid);
						layers[newlayer][oid] = oid;
					}
				}
			}
		}
		for (vector<int>::iterator rmit = toremove.begin(); rmit != toremove.end(); ++rmit)
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
		if (toadd != 0)
		{
			int oid = toadd->getoid();
			char layer = toadd->getlayer();
			objects.add(oid, toadd);
			layers[layer][oid] = oid;
		}
	}

	void Mapobjects::remove(int oid)
	{
		Mapobject* obj = objects.get(oid);
		if (obj != 0)
		{
			char layer = obj->getlayer();
			objects.remove(oid);
			layers[layer].erase(oid);
		}
	}

	Mapobject& Mapobjects::get(int oid)
	{
		return *objects.get(oid);
	}
}
