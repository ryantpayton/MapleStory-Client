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
#include "LayerData.h"
#include "nx.hpp"
#include "bitmap.hpp"

namespace Gameplay
{
	LayerData::LayerData() {}

	LayerData::~LayerData() {}

	void LayerData::clear()
	{
		tilesets.clear();
		objs.clear();
	}

	void LayerData::update(uint16_t dpf)
	{
		for (map<size_t, Animation*>::iterator anit = objs.getbegin(); anit != objs.getend(); ++anit)
		{
			anit->second->update(dpf);
		}
	}

	const Tileset& LayerData::gettileset(string name)
	{
		node src = nl::nx::map["Tile"][name + ".img"];
		if (src.size() > 0)
		{
			if (!tilesets.contains(name))
			{
				tilesets.add(name, new Tileset(src));
			}
			return *tilesets.get(name);
		}
		else
		{
			return nulltileset;
		}
	}

	const Animation& LayerData::getobj(node src)
	{
		if (src["0"].data_type() == node::type::bitmap)
		{
			size_t id = src["0"].get_bitmap().id();
			if (!objs.contains(id))
			{
				objs.add(id, new Animation(src));
			}
			return *objs.get(id);
		}
		else
		{
			return nullobj;
		}
	}
}
