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

namespace Character
{
	// The player's inventory.
	class Inventory
	{
	public:
		// Inventorytypes used by the server.
		enum InvType
		{
			EQUIPPED = -1,
			NONE = 0,
			EQUIP = 1,
			USE = 2,
			SETUP = 3,
			ETC = 4,
			CASH = 5
		};

		Inventory();
		~Inventory();

		// Recalculate sums of equip stats.
		void recalcstats();
		// Set the meso amount.
		void setmeso(int64_t meso);
		// Set the number of slots for a given inventory.
		void setslots(InvType type, uint8_t value);


		void additem(
			InvType type, int16_t slot, int32_t itemid, bool cash,
			int64_t uniqueid, int64_t expire, uint16_t coundt, string owner, int16_t flag
			);
		void addpet(
			InvType type, int16_t slot, int32_t itemid, bool cash, int64_t uniqueid,
			int64_t expire, string name, int8_t level, int16_t closeness, int8_t fullness
			);
		void addequip(
			InvType type, int16_t slot, int32_t itemid, bool cash, int64_t uniqueid, 
			int64_t expire, uint8_t slots, uint8_t level, map<Equipstat, uint16_t> stats, 
			string owner, int16_t flag, uint8_t itemlevel, uint16_t itemexp, int32_t vicious
			);

		void remove(InvType type, int16_t slot);

		uint16_t getstat(Equipstat type) const;
		int64_t getmeso() const;
		int16_t finditem(InvType type, int32_t itemid) const;
		InvType gettypebyid(int32_t itemid) const;

	private:
		void add(InvType type, int16_t slot, Item* toadd);

		int64_t meso;
		map<InvType, uint8_t> slots;
		map<InvType, map<int16_t, Item*>> inventoryitems;
		map<Equipstat, uint16_t> totalstats;
	};
}

