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
#include "Inventory.h"
#include <numeric>
#include <algorithm>

namespace Character
{
	const ItemData& getitem(int32_t itemid)
	{
		static map<int32_t, ItemData> items;
		if (!items.count(itemid))
		{
			items[itemid] = ItemData(itemid);
		}
		return items[itemid];
	}

	Inventory::Inventory()
	{
		meso = 0;
		slots[EQUIPPED] = 255;
	}

	Inventory::~Inventory() {}

	void Inventory::recalcstats()
	{
		for (Equipstat es = ES_STR; es <= ES_JUMP; es = static_cast<Equipstat>(es + 1))
		{
			totalstats[es] = static_cast<uint16_t>(std::accumulate(
				inventoryitems[EQUIPPED].begin(), 
				inventoryitems[EQUIPPED].end(), 0,
				[es](const uint16_t& val, const std::pair<int16_t, Item*>& itit) {
				return val + reinterpret_cast<Equip*>(itit.second)->getstat(es);
			}));
		}
	}

	void Inventory::setmeso(int64_t m)
	{
		meso = m;
	}

	void Inventory::setslots(InvType type, uint8_t sl)
	{
		slots[type] = sl;
	}

	void Inventory::additem(InvType invtype, int16_t slot, int32_t iid, bool cash,
		int64_t uniqueid, int64_t expire, uint16_t count, string owner, int16_t flag) {

		if (slot >= slots[invtype])
			return;

		const ItemData& idata = getitem(iid);
		if (idata.isloaded())
		{
			add(invtype, slot, new Item(
				idata, iid, cash, uniqueid, expire, count, owner, flag)
				);
		}
	}

	void Inventory::addpet(InvType invtype, int16_t slot, int32_t iid, bool cash, int64_t uniqueid, 
		int64_t expire, string name, int8_t level, int16_t closeness, int8_t fullness) {

		if (slot >= slots[invtype])
			return;

		const ItemData& idata = getitem(iid);
		if (idata.isloaded())
		{
			add(invtype, slot, new Pet(
				idata, iid, cash, uniqueid, expire, name, level, closeness, fullness)
				);
		}
	}

	void Inventory::addequip(InvType invtype, int16_t slot, int32_t iid, bool cash, int64_t uniqueid, 
		int64_t expire, uint8_t equipslots, uint8_t level, map<Equipstat, uint16_t> stats, string owner, 
		int16_t flag, uint8_t ilevel, uint16_t iexp, int32_t vicious) {

		if (slot >= slots[invtype])
			return;

		const ItemData& idata = getitem(iid);
		if (idata.isloaded())
		{
			add(invtype, slot, new Equip(
				idata, iid, cash, uniqueid, expire, equipslots, level, stats, owner, flag, ilevel, iexp, vicious)
				);
		}
	}

	void Inventory::add(InvType type, int16_t slot, Item* toadd)
	{
		if (inventoryitems[type].count(slot))
			remove(type, slot);

		inventoryitems[type][slot] = toadd;
	}

	void Inventory::remove(InvType type, int16_t slot)
	{
		if (slot >= slots[type])
			return;

		delete inventoryitems[type][slot];
		inventoryitems[type][slot] = nullptr;
		inventoryitems[type].erase(slot);
	}

	uint16_t Inventory::getstat(Equipstat type) const
	{
		return totalstats.count(type) ? totalstats.at(type) : 0;
	}

	int64_t Inventory::getmeso() const
	{
		return meso;
	}

	int16_t Inventory::finditem(InvType type, int32_t itemid) const
	{
		if (inventoryitems.count(type) == 0)
			return -1;

		auto result = std::find_if(
			inventoryitems.at(type).begin(), 
			inventoryitems.at(type).end(),
			[itemid](const std::pair<int16_t, Item*>& itit) {
			return itit.second->getid() == itemid;
		});

		if (result == inventoryitems.at(type).end())
			return -1;
		else
			return result->first;
	}

	Inventory::InvType Inventory::gettypebyid(int32_t itemid) const
	{
		static const InvType typesbyid[6] =
		{
			NONE, EQUIP, USE, SETUP, ETC, CASH
		};

		int32_t prefix = itemid / 1000000;
		if (prefix > 0 && prefix < 6)
			return typesbyid[prefix];
		else
			return NONE;
	}
}
