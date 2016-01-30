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
#include "Item.h"
#include "Pet.h"
#include "Equip.h"
#include "Character\CharStats.h"

namespace Character
{
	// The player's inventory.
	class Inventory
	{
	public:
		// Inventorytypes used by the server.
		enum InvType : int8_t
		{
			EQUIPPED = -1,
			NONE = 0,
			EQUIP = 1,
			USE = 2,
			SETUP = 3,
			ETC = 4,
			CASH = 5
		};

		enum Movetype : int8_t
		{
			MOVE_NONE = -1,
			MOVE_INTERNAL = 0,
			MOVE_UNEQUIP = 1,
			MOVE_EQUIP = 2
		};

		Inventory();
		~Inventory();

		// Recalculate sums of equip stats.
		void recalcstats();
		// Add total stats to the character stats.
		void addtotalsto(CharStats& stats) const;
		// Set the meso amount.
		void setmeso(int64_t meso);
		// Set the number of slots for a given inventory.
		void setslots(InvType type, uint8_t value);

		// Add a general item.
		void additem(
			InvType type, int16_t slot, int32_t itemid, bool cash,
			int64_t uniqueid, int64_t expire, uint16_t coundt, string owner, int16_t flag
			);
		// Add a pet item.
		void addpet(
			InvType type, int16_t slot, int32_t itemid, bool cash, int64_t uniqueid,
			int64_t expire, string name, int8_t level, int16_t closeness, int8_t fullness
			);
		// Add an equip item.
		void addequip(
			InvType type, int16_t slot, int32_t itemid, bool cash, int64_t uniqueid, 
			int64_t expire, uint8_t slots, uint8_t level, map<Equipstat::Value, uint16_t> stats, 
			string owner, int16_t flag, uint8_t itemlevel, uint16_t itemexp, int32_t vicious
			);
		// Remove an item.
		void remove(InvType type, int16_t slot);
		// Swap two items.
		void swap(InvType firsttype, int16_t firstslot, InvType secondtype, int16_t secondslot);
		// Modify the inventory with info from a packet.
		void modify(InvType type, int16_t pos, int8_t mode, int16_t arg, Movetype move);
		// Change the quantity of an item.
		void changecount(InvType type, int16_t slot, int16_t count);

		// Return if an equip is equipped in the specfied slot.
		bool hasequipped(Slot::Value slot) const;
		// Return the number of slots for the specified inventory.
		uint8_t getslots(InvType type) const;
		// Return a total stat.
		uint16_t getstat(Equipstat::Value type) const;
		// Return the amount of meso.
		int64_t getmeso() const;
		// Find a free slot for the specified equip.
		Slot::Value findequipslot(int32_t itemid) const;
		// Find a free slot in the specfified inventory.
		int16_t findslot(InvType type) const;
		// Return the first slot which contains the specified item.
		int16_t finditem(InvType type, int32_t itemid) const;
		// Return the inventory type by itemid.
		InvType gettypebyid(int32_t itemid) const;
		// Return the inventory type by value.
		InvType typebyvalue(int8_t value) const;
		// Return the inventory type by value.
		Movetype movetypebyvalue(int8_t value) const;

		const Item* getitem(InvType type, int16_t slot) const;
		const Equip* getequip(InvType type, int16_t slot) const;

	private:
		void add(InvType type, int16_t slot, Item* toadd);

		int64_t meso;
		map<InvType, uint8_t> slots;
		map<InvType, map<int16_t, Item*>> inventoryitems;
		map<Equipstat::Value, uint16_t> totalstats;
	};
}

