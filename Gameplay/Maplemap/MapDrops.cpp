/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 Daniel Allendorf                                               //
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
#include "MapDrops.h"
#include "ItemDrop.h"
#include "MesoDrop.h"

#include "..\..\Constants.h"
#include "..\..\Data\DataFactory.h"

#include "nlnx\nx.hpp"

namespace jrc
{
	MapDrops::MapDrops() 
	{
		lootenabled = false;
	}

	void MapDrops::sendspawn(const DropSpawn& spawn)
	{
		int32_t oid = spawn.getoid();
		Optional<Drop> drop = getdrop(oid);
		if (drop)
		{
			drop->makeactive();
		}
		else
		{
			int32_t itemid = spawn.getitemid();
			bool meso = spawn.ismeso();
			if (meso)
			{
				MesoType mesotype = (itemid > 999) ? BAG : (itemid > 99)
					? BUNDLE : (itemid > 49) ? GOLD : BRONZE;
				if (mesoicons.count(mesotype))
				{
					MesoDrop* newdrop = spawn.instantiate(&mesoicons[mesotype]);
					add(newdrop);
				}
				
			}
			else
			{
				auto& itemdata = DataFactory::get().getitemdata(itemid);
				if (itemdata.isloaded())
				{
					auto& icon = itemdata.geticon(true);
					ItemDrop* newdrop = spawn.instantiate(&icon);
					add(newdrop);
				}
			}
		}
	}

	void MapDrops::removedrop(int32_t oid, int8_t mode, const PhysicsObject* looter)
	{
		get(oid)
			.reinterpret<Drop>()
			.ifpresent(&Drop::expire, mode, looter);
	}

	void MapDrops::update(const Physics& physics)
	{
		MapObjects::update(physics);

		lootenabled = true;

		for (auto& msani : mesoicons)
		{
			msani.second.update(Constants::TIMESTEP);
		}
	}

	const Drop* MapDrops::findinrange(Point<int16_t> playerpos)
	{
		if (!lootenabled)
			return nullptr;

		for (auto& mmo : objects)
		{
			Optional<const Drop> drop = Optional<MapObject>(mmo.second.get())
				.reinterpret<const Drop>();
			if (drop && drop->bounds().contains(playerpos))
			{
				lootenabled = false;
				return drop.get();
			}
		}
		return nullptr;
	}

	Optional<Drop> MapDrops::getdrop(int32_t oid)
	{
		return get(oid)
			.reinterpret<Drop>();
	}


	void MapDrops::init()
	{
		nl::node src = nl::nx::item["Special"]["0900.img"];

		mesoicons[BRONZE] = src["09000000"]["iconRaw"];
		mesoicons[GOLD] = src["09000001"]["iconRaw"];
		mesoicons[BUNDLE] = src["09000002"]["iconRaw"];
		mesoicons[BAG] = src["09000003"]["iconRaw"];
	}
	std::unordered_map<MapDrops::MesoType, Animation> MapDrops::mesoicons;
}