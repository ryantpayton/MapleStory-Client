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

namespace Character
{
	using Data::DataFactory;

	Inventory::Inventory()
	{
		bulletslot = 0;
		meso = 0;
		slots[EQUIPPED] = 255;
	}

	Inventory::~Inventory() 
	{
		for (auto& sub : inventories)
		{
			Type type = sub.first;
			for (auto& item : sub.second)
			{
				int16_t slot = item.first;
				remove(type, slot);
			}
		}
	}

	void Inventory::recalcstats(Weapon::Type type)
	{
		for (auto it = Equipstat::it(); it.hasnext(); it.increment())
		{
			Equipstat::Value es = it.get();
			totalstats[es] = static_cast<uint16_t>(std::accumulate(
				inventories[EQUIPPED].begin(), 
				inventories[EQUIPPED].end(), 0,
				[es](const uint16_t& val, const std::pair<int16_t, Item*>& itit) {
				return val + reinterpret_cast<Equip*>(itit.second)->getstat(es);
			}));
		}

		int32_t prefix = 0;
		switch (type)
		{
		case Weapon::BOW:
			prefix = 2060;
			break;
		case Weapon::CROSSBOW:
			prefix = 2061;
			break;
		case Weapon::CLAW:
			prefix = 2070;
			break;
		case Weapon::GUN:
			prefix = 2330;
			break;
		}

		bulletslot = 0;
		if (prefix > 0)
		{
			for (auto& use : inventories[USE])
			{
				Item* item = use.second;
				if (item && item->getid() / 1000 == prefix && item->getcount() > 0)
				{
					bulletslot = use.first;
					break;
				}
			}
		}

		if (bulletslot)
		{
			int16_t watkbonus = getitem(USE, bulletslot)
				.transform(DataFactory::get(), &DataFactory::getbulletdata, &Item::getid)
				.mapordefault(&BulletData::getwatk, int16_t(0));

			totalstats[Equipstat::WATK] += watkbonus;
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

	void Inventory::setslots(Type type, uint8_t sl)
	{
		slots[type] = sl;
	}

	void Inventory::additem(Type invtype, int16_t slot, int32_t iid, bool cash,
		int64_t uniqueid, int64_t expire, uint16_t count, string owner, int16_t flag) {

		if (slot >= slots[invtype])
			return;

		const ItemData& idata = DataFactory::get().getitemdata(iid);
		if (idata.isloaded())
		{
			add(invtype, slot, new Item(
				idata, iid, cash, uniqueid, expire, count, owner, flag)
				);
		}
	}

	void Inventory::addpet(Type invtype, int16_t slot, int32_t iid, bool cash, int64_t uniqueid, 
		int64_t expire, string name, int8_t level, int16_t closeness, int8_t fullness) {

		if (slot >= slots[invtype])
			return;

		const ItemData& idata = DataFactory::get().getitemdata(iid);
		if (idata.isloaded())
		{
			add(invtype, slot, new Pet(
				idata, iid, cash, uniqueid, expire, name, level, closeness, fullness)
				);
		}
	}

	void Inventory::addequip(Type invtype, int16_t slot, int32_t iid, bool cash, int64_t uniqueid, 
		int64_t expire, uint8_t equipslots, uint8_t level, map<Equipstat::Value, uint16_t> stats, string owner, 
		int16_t flag, uint8_t ilevel, uint16_t iexp, int32_t vicious) {

		if (slot >= slots[invtype])
			return;

		const ItemData& idata = DataFactory::get().getitemdata(iid);
		if (idata.isloaded())
		{
			add(invtype, slot, new Equip(
				idata, iid, cash, uniqueid, expire, equipslots, level, stats, owner, flag, ilevel, iexp, vicious)
				);
		}
	}

	void Inventory::add(Type type, int16_t slot, Item* toadd)
	{
		if (inventories[type].count(slot))
			remove(type, slot);

		inventories[type][slot] = toadd;
	}

	void Inventory::remove(Type type, int16_t slot)
	{
		if (slot >= slots[type])
			return;

		delete inventories[type][slot];
		inventories[type][slot] = nullptr;
		inventories[type].erase(slot);
	}

	void Inventory::swap(Type firsttype, int16_t firstslot, Type secondtype, int16_t secondslot)
	{
		Item* temp = inventories[firsttype][firstslot];
		inventories[firsttype][firstslot] = inventories[secondtype][secondslot];
		inventories[secondtype][secondslot] = temp;

		if (inventories[firsttype][firstslot] == nullptr)
			inventories[firsttype].erase(firstslot);
		else if (inventories[secondtype][secondslot] == nullptr)
			inventories[secondtype].erase(secondslot);
	}

	bool Inventory::changecount(Type type, int16_t slot, int16_t count)
	{
		Optional<Item> item = getitem(type, slot);
		if (item)
		{
			int16_t cur = item->getcount();
			item->setcount(count);
			return count > cur;
		}
		else
		{
			return false;
		}
	}

	void Inventory::modify(Type type, int16_t slot, int8_t mode, int16_t arg, Movement move)
	{
		if (slot < 0)
		{
			slot = -slot;
			type = EQUIPPED;
		}
		arg = (arg < 0) ? -arg : arg;

		switch (mode)
		{
		case CHANGECOUNT:
			changecount(type, slot, arg);
			break;
		case SWAP:
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
		case REMOVE:
			remove(type, slot);
			break;
		}
	}

	uint8_t Inventory::getslots(Type type) const
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

	bool Inventory::hasprojectile() const
	{
		return bulletslot > 0;
	}

	bool Inventory::hasequipped(Equipslot::Value slot) const
	{
		int16_t value = Equipslot::valueof(slot);
		return inventories.at(Inventory::EQUIPPED).count(value) > 0;
	}

	int16_t Inventory::getbulletslot() const
	{
		return bulletslot;
	}

	uint16_t Inventory::getbulletcount() const
	{
		return getitem(Inventory::USE, bulletslot)
			.map(&Item::getcount);
	}

	int32_t Inventory::getbulletid() const
	{
		return getitem(Inventory::USE, bulletslot)
			.map(&Item::getid);
	}

	Equipslot::Value Inventory::findequipslot(int32_t itemid) const
	{
		const Clothing& cloth = DataFactory::get().getclothing(itemid);
		if (!cloth.isloaded())
			return Equipslot::NONE;

		Equipslot::Value eqslot = cloth.geteqslot();
		if (eqslot == Equipslot::RING)
		{
			if (!hasequipped(Equipslot::RING2))
				return Equipslot::RING2;

			if (!hasequipped(Equipslot::RING3))
				return Equipslot::RING3;

			if (!hasequipped(Equipslot::RING4))
				return Equipslot::RING4;

			return Equipslot::RING;
		}
		else
		{
			return eqslot;
		}
	}

	int16_t Inventory::findslot(Type type) const
	{
		uint8_t numslots = slots.at(type);
		for (uint8_t i = 1; i < numslots; i++)
		{
			if (inventories.count(type) == 0)
				return 1;

			if (inventories.at(type).count(i) == 0)
				return i;
		}
		return 0;
	}

	int16_t Inventory::finditem(Type type, int32_t itemid) const
	{
		if (inventories.count(type) == 0)
			return -1;

		for (auto& item : inventories.at(type))
		{
			int32_t id = item.second->getid();
			if (id == itemid)
				return item.first;
		}
		return -1;
	}

	Optional<Item> Inventory::getitem(Type type, int16_t slot) const
	{
		if (!inventories.count(type))
			return Optional<Item>();

		if (!inventories.at(type).count(slot))
			return Optional<Item>();

		return inventories.at(type).at(slot);
	}

	Optional<Equip> Inventory::getequip(Type type, int16_t slot) const
	{
		if (type != EQUIP && type != EQUIPPED)
			return Optional<Equip>();

		return getitem(type, slot)
			.reinterpret<Equip>();
	}
}
