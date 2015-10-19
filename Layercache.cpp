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
#include "Layercache.h"
#include "nx.hpp"
#include "bitmap.hpp"

namespace Gameplay
{
	void Layercache::clear()
	{
		tilesets.clear();
		objs.clear();
	}

	void Layercache::update(short dpf)
	{
		for (map<size_t, Animation*>::iterator anit = objs.getbegin(); anit != objs.getend(); ++anit)
		{
			anit->second->update(dpf);
		}
	}

	Tileset* Layercache::gettileset(string name)
	{
		if (!tilesets.contains(name))
		{
			node src = nx::map["Tile"][name + ".img"];
			if (src.size() > 0)
			{
				tilesets.add(name, new Tileset(src));
			}
		}
		return tilesets.get(name);
	}

	Animation& Layercache::getobj(node src)
	{
		size_t id = src["0"].get_bitmap().id();
		if (!objs.contains(id))
		{
			objs.add(id, new Animation(src));
		}
		return *objs.get(id);
	}
}
