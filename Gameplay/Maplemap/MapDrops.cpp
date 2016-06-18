/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
	void MesoIcons::init()
	{
		nl::node src = nl::nx::item["Special"]["0900.img"];

		animations[BRONZE] = src["09000000"]["iconRaw"];
		animations[GOLD] = src["09000001"]["iconRaw"];
		animations[BUNDLE] = src["09000002"]["iconRaw"];
		animations[BAG] = src["09000003"]["iconRaw"];
	}

	void MesoIcons::update()
	{
		for (Animation& animation : animations)
		{
			animation.update();
		}
	}

	const Animation* MesoIcons::get(MesoIcons::Type type) const
	{
		return &animations[type];
	}


	MapDrops::MapDrops() 
	{
		lootenabled = false;
	}

	void MapDrops::send_spawn(const DropSpawn& spawn)
	{
		int32_t oid = spawn.get_oid();
		Optional<Drop> drop = get_drop(oid);
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
				MesoIcons::Type mesotype = (itemid > 999) ? MesoIcons::BAG : (itemid > 99)
					? MesoIcons::BUNDLE : (itemid > 49) ? MesoIcons::GOLD : MesoIcons::BRONZE;
				drops.add(
					spawn.instantiate(meso_icons.get(mesotype))
				);
			}
			else
			{
				auto& itemdata = DataFactory::get().get_itemdata(itemid);
				if (itemdata.is_loaded())
				{
					auto& icon = itemdata.geticon(true);
					drops.add(
						spawn.instantiate(&icon)
					);
				}
			}
		}
	}

	void MapDrops::remove_drop(int32_t oid, int8_t mode, const PhysicsObject* looter)
	{
		drops.get(oid)
			.reinterpret<Drop>()
			.if_present(&Drop::expire, mode, looter);
	}

	void MapDrops::clear()
	{
		drops.clear();
	}

	void MapDrops::draw(int8_t layer, double viewx, double viewy, float alpha) const
	{
		drops.draw(layer, viewx, viewy, alpha);
	}

	void MapDrops::update(const Physics& physics)
	{
		drops.update(physics);
		meso_icons.update();

		lootenabled = true;
	}

	const Drop* MapDrops::findinrange(Point<int16_t> playerpos)
	{
		if (!lootenabled)
			return nullptr;

		for (auto& mmo : drops)
		{
			const Drop* drop = static_cast<const Drop*>(mmo.second.get());
			if (drop && drop->bounds().contains(playerpos))
			{
				lootenabled = false;
				return drop;
			}
		}
		return nullptr;
	}

	Optional<Drop> MapDrops::get_drop(int32_t oid)
	{
		return drops.get(oid)
			.reinterpret<Drop>();
	}


	void MapDrops::init()
	{
		meso_icons.init();
	}

	MesoIcons MapDrops::meso_icons;
}