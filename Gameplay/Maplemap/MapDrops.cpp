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
#include "Constants.h"

#include "Audio\AudioPlayer.h"
#include "Data\DataFactory.h"

#include "nlnx\nx.hpp"

#include <string>

namespace Gameplay
{
	using Audio::AudioPlayer;
	using Data::DataFactory;

	MapDrops::MapDrops() 
	{
		lootenabled = false;
	}

	void MapDrops::adddrop(int32_t oid, int32_t itemid, bool meso, int32_t owner, Point<int16_t> pos, 
		Point<int16_t> dest, int8_t type, int8_t mode, bool playerdrop) {

		Optional<MapObject> mmo = get(oid);
		if (mmo)
		{
			mmo->setactive(true);
		}
		else
		{
			if (meso)
			{
				MesoType mstype = (itemid > 999) ? MES_BAG : (itemid > 99)
					? MES_BUNDLE : (itemid > 49) ? MES_GOLD : MES_BRONZE;
				if (!mesos.count(mstype))
				{
					string msstr = std::to_string(mstype);
					mesos[mstype] = Animation(nl::nx::item["Special"]["0900.img"]["0900000" + msstr]["iconRaw"]);
				}
				add(new MesoDrop(oid, owner, pos, dest, type, mode, &mesos[mstype], playerdrop));
			}
			else
			{
				const Character::ItemData& itemdata = DataFactory::get().getitemdata(itemid);
				if (!itemdata.isloaded())
					return;

				add(new ItemDrop(oid, owner, pos, dest, type, mode, itemdata.geticon(true), itemid, playerdrop));
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

		if (!lootenabled)
			lootenabled = true;

		for (auto& msani : mesos)
		{
			msani.second.update(Constants::TIMESTEP / 2);
		}
	}

	const Drop* MapDrops::findinrange(rectangle2d<int16_t> range)
	{
		if (!lootenabled)
			return nullptr;

		for (auto& mmo : objects)
		{
			if (mmo.second == nullptr)
				continue;

			Drop* drop = reinterpret_cast<Drop*>(mmo.second.get());
			if (range.overlaps(drop->bounds()))
			{
				lootenabled = false;
				return drop;
			}
		}
		return nullptr;
	}
}