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
#pragma once
#include "Item.h"
#include "Pet.h"
#include "Equip.h"

#include "..\CharStats.h"

#include "..\..\Console.h"
#include "..\..\Util\EnumMap.h"
#include "..\..\Util\Optional.h"

namespace jrc
{
	// The player's inventory.
	class Inventory
	{
	public:
		// Inventorytypes.
		enum Type : int8_t
		{
			NONE = 0,
			EQUIP = 1,
			USE = 2,
			SETUP = 3,
			ETC = 4,
			CASH = 5,
			EQUIPPED = 6,
			NUM_TYPES
		};

		// Return the inventory type by itemid.
		static Type typebyid(int32_t itemid)
		{
			static const Type typesbyid[6] =
			{
				NONE, EQUIP, USE, SETUP, ETC, CASH
			};

			int32_t prefix = itemid / 1000000;
			return (prefix > 0 && prefix < 6) ? typesbyid[prefix] : NONE;
		}

		// Return the inventory type by value.
		static Type typebyvalue(int8_t value)
		{
			switch (value)
			{
			case -1:
				return EQUIPPED;
			case 1:
				return EQUIP;
			case 2:
				return USE;
			case 3:
				return SETUP;
			case 4:
				return ETC;
			case 5:
				return CASH;
			}

			Console::get().print("Unknown inventory type: " + std::to_string(value));
			return NONE;
		}

		enum Movement : int8_t
		{
			MOVE_NONE = -1,
			MOVE_INTERNAL = 0,
			MOVE_UNEQUIP = 1,
			MOVE_EQUIP = 2
		};

		enum Modification : int8_t
		{
			ADD,
			CHANGECOUNT,
			SWAP,
			REMOVE,
			ADDCOUNT
		};

		// Return the move type by value.
		static Movement movementbyvalue(int8_t value)
		{
			if (value >= MOVE_INTERNAL && value <= MOVE_EQUIP)
				return static_cast<Movement>(value);

			Console::get().print("Unknown move type: " + std::to_string(value));
			return MOVE_NONE;
		}

		Inventory();
		~Inventory();

		// Recalculate sums of equip stats.
		void recalc_stats(Weapon::Type weapontype);
		// Add total stats to the character stats.
		void add_equipstats(CharStats& stats) const;
		// Set the meso amount.
		void set_meso(int64_t meso);
		// Set the number of slots for a given inventory.
		void set_slots(Type type, uint8_t value);

		// Add a general item.
		void add_item(
			Type type, int16_t slot, int32_t itemid, bool cash,
			int64_t uniqueid, int64_t expire, uint16_t coundt, const std::string& owner, int16_t flag
			);
		// Add a pet item.
		void add_pet(
			Type type, int16_t slot, int32_t itemid, bool cash, int64_t uniqueid,
			int64_t expire, const std::string& name, int8_t level, int16_t closeness, int8_t fullness
			);
		// Add an equip item.
		void add_equip(
			Type type, int16_t slot, int32_t itemid, bool cash, int64_t uniqueid, 
			int64_t expire, uint8_t slots, uint8_t level, const EnumMap<Equipstat::Value, uint16_t>& stats,
			const std::string& owner, int16_t flag, uint8_t itemlevel, uint16_t itemexp, int32_t vicious
			);

		// Remove an item.
		void remove(Type type, int16_t slot);
		// Swap two items.
		void swap(Type firsttype, int16_t firstslot, Type secondtype, int16_t secondslot);
		// Modify the inventory with info from a packet.
		void modify(Type type, int16_t pos, int8_t mode, int16_t arg, Movement movement);
		// Change the quantity of an item.
		bool change_count(Type type, int16_t slot, int16_t count);

		// Check if the use inventory contains at least one projectile.
		bool has_projectile() const;
		// Return if an equip is equipped in the specfied slot.
		bool has_equipped(Equipslot::Value slot) const;
		// Return the currently active projectile slot.
		int16_t get_bulletslot() const;
		// Return the count of the currently active projectile.
		uint16_t get_bulletcount() const;
		// Return the itemid of the currently active projectile.
		int32_t get_bulletid() const;
		// Return the number of slots for the specified inventory.
		uint8_t get_slots(Type type) const;
		// Return a total stat.
		uint16_t get_stat(Equipstat::Value type) const;
		// Return the amount of meso.
		int64_t get_meso() const;
		// Find a free slot for the specified equip.
		Equipslot::Value find_equipslot(int32_t itemid) const;
		// Find a free slot in the specfified inventory.
		int16_t find_free_slot(Type type) const;
		// Return the first slot which contains the specified item.
		int16_t find_item(Type type, int32_t itemid) const;

		// Obtain a pointer to the item at the specified type and slot.
		Optional<Item> get_item(Type type, int16_t slot) const;
		// Obtain a pointer to the equip at the specified type and slot.
		Optional<Equip> get_equip(Type type, int16_t slot) const;

	private:
		void add(Type type, int16_t slot, Item* toadd);

		int64_t meso;
		EnumMap<Type, uint8_t, NUM_TYPES> slots;
		EnumMap<Type, std::map<int16_t, Item*>, NUM_TYPES> inventories;
		EnumMap<Equipstat::Value, uint16_t> totalstats;

		int16_t bulletslot;
	};
}

