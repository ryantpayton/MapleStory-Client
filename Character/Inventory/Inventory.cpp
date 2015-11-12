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
	}

	Inventory::~Inventory() {}

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

		const ItemData& idata = getitem(iid);
		if (idata.isloaded())
		{
			inventoryitems[invtype].add(slot, 
				new Item(idata, iid, cash, uniqueid, expire, count, owner, flag));
		}
	}

	void Inventory::addpet(InvType invtype, int16_t slot, int32_t iid, bool cash, int64_t uniqueid, 
		int64_t expire, string name, int8_t level, int16_t closeness, int8_t fullness) {

		const ItemData& idata = getitem(iid);
		if (idata.isloaded())
		{
			inventoryitems[invtype].add(slot, 
				new Pet(idata, iid, cash, uniqueid, expire, name, level, closeness, fullness));
		}
	}

	void Inventory::addequip(InvType invtype, int16_t slot, int32_t iid, bool cash, int64_t uniqueid, int64_t expire, uint8_t slots, 
		uint8_t level, map<Equipstat, uint16_t> stats, string owner, int16_t flag, uint8_t ilevel, uint16_t iexp, int32_t vicious) {

		const ItemData& idata = getitem(iid);
		if (idata.isloaded())
		{
			inventoryitems[invtype].add(slot, 
				new Equip(idata, iid, cash, uniqueid, expire, slots, level, stats, owner, flag, ilevel, iexp, vicious));
		}
	}
}
