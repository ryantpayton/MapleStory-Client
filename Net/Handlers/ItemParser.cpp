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
#include "ItemParser.h"

namespace jrc
{
	namespace ItemParser
	{
		// Parse a normal item from a packet.
		void additem(InPacket& recv, Inventory::Type invtype, int16_t slot, int32_t id, Inventory& inventory)
		{
			// Read all item stats.
			bool cash = recv.read_bool();
			int64_t uniqueid = cash ? recv.read_long() : -1;
			int64_t expire = recv.read_long();
			int16_t count = recv.read_short();
			std::string owner = recv.read_string();
			int16_t flag = recv.read_short();

			// If the item is a rechargable projectile, some additional bytes are sent.
			if ((id / 10000 == 233) || (id / 10000 == 207))
			{
				recv.skip(8);
			}

			inventory.additem(invtype, slot, id, cash, uniqueid, expire, count, owner, flag);
		}

		// Parse a pet from a packet.
		void addpet(InPacket& recv, Inventory::Type invtype, int16_t slot, int32_t id, Inventory& inventory)
		{
			// Read all pet stats.
			bool cash = recv.read_bool();
			int64_t uniqueid = cash ? recv.read_long() : -1;
			int64_t expire = recv.read_long();
			std::string petname = recv.read_padded_string(13);
			int8_t petlevel = recv.read_byte();
			int16_t closeness = recv.read_short();
			int8_t fullness = recv.read_byte();

			// Some unused bytes.
			recv.skip(18);

			inventory.addpet(invtype, slot, id, cash, uniqueid, expire, petname, petlevel, closeness, fullness);
		}

		// Parse an equip from a packet.
		void addequip(InPacket& recv, Inventory::Type invtype, int16_t slot, int32_t id, Inventory& inventory)
		{
			// Read equip information.
			bool cash = recv.read_bool();
			int64_t uniqueid = -1;
			if (cash)
			{
				uniqueid = recv.read_long();
			}
			int64_t expire = recv.read_long();
			uint8_t slots = recv.read_byte();
			uint8_t level = recv.read_byte();

			// Read equip stats.
			EnumMap<Equipstat::Value, uint16_t> stats;
			for (auto es : stats.getkeys())
			{
				stats[es] = recv.read_short();
			}

			// Some more information.
			std::string owner = recv.read_string();
			int16_t flag = recv.read_short();
			uint8_t itemlevel = 0;
			uint16_t itemexp = 0;
			int32_t vicious = 0;
			if (cash)
			{
				// Some unused bytes.
				recv.skip(10);
			}
			else
			{
				recv.read_byte();
				itemlevel = recv.read_byte();
				recv.read_short();
				itemexp = recv.read_short();
				vicious = recv.read_int();
				recv.read_long();
			}

			recv.skip(12);

			if (slot < 0)
			{
				invtype = Inventory::EQUIPPED;
				slot = -slot;
			}

			inventory.addequip(invtype, slot, id, cash, uniqueid, expire, slots,
				level, stats, owner, flag, itemlevel, itemexp, vicious);
		}

		void parseitem(InPacket& recv, Inventory::Type invtype, int16_t slot, Inventory& inventory)
		{
			// Read type and item id.
			recv.read_byte(); // 'type' byte
			int32_t iid = recv.read_int();

			if (invtype == Inventory::EQUIP || invtype == Inventory::EQUIPPED)
			{
				// Parse an equip.
				addequip(recv, invtype, slot, iid, inventory);
			}
			else if (iid >= 5000000 && iid <= 5000102)
			{
				// Parse a pet.
				addpet(recv, invtype, slot, iid, inventory);
			}
			else
			{
				// Parse a normal item.
				additem(recv, invtype, slot, iid, inventory);
			}
		}
	}
}