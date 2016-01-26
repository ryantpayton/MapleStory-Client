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
#include "Data\DataFactory.h"
#include <numeric>
#include <algorithm>

namespace Character
{
	Inventory::Inventory()
	{
		meso = 0;
		slots[EQUIPPED] = 255;
	}

	Inventory::~Inventory() {}

	void Inventory::recalcstats()
	{
		for (auto it = Equipstat::it(); it.hasnext(); it.increment())
		{
			Equipstat::Value es = it.get();
			totalstats[es] = static_cast<uint16_t>(std::accumulate(
				inventoryitems[EQUIPPED].begin(), 
				inventoryitems[EQUIPPED].end(), 0,
				[es](const uint16_t& val, const std::pair<int16_t, Item*>& itit) {
				return val + reinterpret_cast<Equip*>(itit.second)->getstat(es);
			}));
		}
	}

	void Inventory::addtotalsto(CharStats& stats) const
	{
		for (auto& tsit : totalstats)
		{
			Equipstat::Value es = tsit.first;
			stats.addtotal(es, tsit.second);
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

		const ItemData& idata = Data::getitemdata(iid);
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

		const ItemData& idata = Data::getitemdata(iid);
		if (idata.isloaded())
		{
			add(invtype, slot, new Pet(
				idata, iid, cash, uniqueid, expire, name, level, closeness, fullness)
				);
		}
	}

	void Inventory::addequip(InvType invtype, int16_t slot, int32_t iid, bool cash, int64_t uniqueid, 
		int64_t expire, uint8_t equipslots, uint8_t level, map<Equipstat::Value, uint16_t> stats, string owner, 
		int16_t flag, uint8_t ilevel, uint16_t iexp, int32_t vicious) {

		if (slot >= slots[invtype])
			return;

		const ItemData& idata = Data::getitemdata(iid);
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

	void Inventory::swap(InvType firsttype, int16_t firstslot, InvType secondtype, int16_t secondslot)
	{
		Item* temp = inventoryitems[firsttype][firstslot];
		inventoryitems[firsttype][firstslot] = inventoryitems[secondtype][secondslot];
		inventoryitems[secondtype][secondslot] = temp;

		if (inventoryitems[firsttype][firstslot] == nullptr)
			inventoryitems[firsttype].erase(firstslot);
	}

	void Inventory::changecount(InvType type, int16_t slot, int16_t count)
	{
		Item* item = inventoryitems[type][slot];
		if (item)
			item->setcount(count);
	}

	void Inventory::modify(InvType type, int16_t slot, int8_t mode, int16_t arg, Movetype move)
	{
		if (slot < 0)
		{
			slot = -slot;
			type = EQUIPPED;
		}
		arg = (arg < 0) ? -arg : arg;

		switch (mode)
		{
		case 1:
			changecount(type, slot, arg);
			break;
		case 2:
			switch (move)
			{
			case MOVE_INTERNAL:
				swap(type, slot, type, arg);
				break;
			case MOVE_UNEQUIP:
				swap(EQUIPPED, slot, EQUIP, arg);
				break;
			case MOVE_EQUIP:
				swap(EQUIP, slot, EQUIPPED, arg);
				break;
			}
			break;
		case 3:
			remove(type, slot);
			break;
		}
	}

	uint8_t Inventory::getslots(InvType type) const
	{
		if (slots.count(type))
			return slots.at(type);
		else
			return 0;
	}

	uint16_t Inventory::getstat(Equipstat::Value type) const
	{
		return totalstats.count(type) ? totalstats.at(type) : 0;
	}

	int64_t Inventory::getmeso() const
	{
		return meso;
	}

	bool Inventory::hasequipped(Clothing::Slot slot) const
	{
		int16_t intslot = static_cast<int16_t>(slot);
		return inventoryitems.at(Inventory::EQUIPPED).count(intslot) > 0;
	}

	Clothing::Slot Inventory::findequipslot(int32_t itemid) const
	{
		const Clothing& cloth = Data::getclothing(itemid);
		if (!cloth.isloaded())
			return Clothing::NONE;

		Clothing::Slot eqslot = cloth.geteqslot();
		if (eqslot == Clothing::RING)
		{
			if (!hasequipped(Clothing::RING2))
				return Clothing::RING2;

			if (!hasequipped(Clothing::RING3))
				return Clothing::RING3;

			if (!hasequipped(Clothing::RING4))
				return Clothing::RING4;

			return Clothing::RING;
		}
		else
		{
			return eqslot;
		}
	}

	int16_t Inventory::findslot(InvType type) const
	{
		uint8_t numslots = slots.at(type);
		for (uint8_t i = 1; i < numslots; i++)
		{
			if (inventoryitems.count(type) == 0)
				return 1;

			if (inventoryitems.at(type).count(i) == 0)
				return i;
		}
		return 0;
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

	Inventory::InvType Inventory::typebyvalue(int8_t value) const
	{
		if (value < EQUIPPED || value > CASH)
			return NONE;
		else
			return static_cast<InvType>(value);
	}

	Inventory::Movetype Inventory::movetypebyvalue(int8_t value) const
	{
		if (value < MOVE_INTERNAL || value > MOVE_EQUIP)
			return MOVE_NONE;
		else
			return static_cast<Movetype>(value);
	}

	const Item* Inventory::getitem(InvType type, int16_t slot) const
	{
		if (inventoryitems.count(type) == 0)
			return nullptr;

		if (inventoryitems.at(type).count(slot) == 0)
			return nullptr;

		return inventoryitems.at(type).at(slot);
	}

	const Equip* Inventory::getequip(InvType type, int16_t slot) const
	{
		if (type != EQUIP && type != EQUIPPED)
			return nullptr;

		const Item* item = getitem(type, slot);
		if (item)
			return reinterpret_cast<const Equip*>(item);
		else
			return nullptr;
	}
}
