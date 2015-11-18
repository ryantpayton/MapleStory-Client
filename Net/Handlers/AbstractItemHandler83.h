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
#include "Net\PacketHandler.h"
#include "Character\Inventory\Inventory.h"

namespace Net
{
	using::Character::Item;
	using::Character::Pet;
	using::Character::Equip;
	using::Character::Inventory;

	// A base class for PacketHandlers which need to create items from a packet.
	class AbstractItemHandler83 : public PacketHandler
	{
	protected:
		// Parses an item from a packet, using the datacache to obtain the item data.
		void parseitem(InPacket& recv, Inventory::InvType invtype, int16_t slot, Inventory& inventory) const
		{
			// Read type and item id.
			recv.readbyte(); // 'type' byte
			int32_t iid = recv.readint();

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
	private:
		// Parse a normal item from a packet.
		void additem(InPacket& recv, Inventory::InvType invtype, int16_t slot, int32_t id, Inventory& inventory) const
		{
			// Read all item stats.
			bool cash = recv.readbool();
			int64_t uniqueid = cash ? recv.readlong() : -1;
			int64_t expire = recv.readlong();
			int16_t count = recv.readshort();
			string owner = recv.readascii();
			int16_t flag = recv.readshort();

			// If the item is a rechargable projectile, some additional bytes are sent.
			if ((id / 10000 == 233) || (id / 10000 == 207))
			{
				recv.skip(8);
			}

			inventory.additem(invtype, slot, id, cash, uniqueid, expire, count, owner, flag);
		}

		// Parse a pet from a packet.
		void addpet(InPacket& recv, Inventory::InvType invtype, int16_t slot, int32_t id, Inventory& inventory) const
		{
			// Read all pet stats.
			bool cash = recv.readbool();
			int64_t uniqueid = cash ? recv.readlong() : -1;
			int64_t expire = recv.readlong();
			string petname = recv.readpadascii(13);
			int8_t petlevel = recv.readbyte();
			int16_t closeness = recv.readshort();
			int8_t fullness = recv.readbyte();

			// Some unused bytes.
			recv.skip(18);

			inventory.addpet(invtype, slot, id, cash, uniqueid, expire, petname, petlevel, closeness, fullness);
		}

		// Parse an equip from a packet.
		void addequip(InPacket& recv, Inventory::InvType invtype, int16_t slot, int32_t id, Inventory& inventory) const
		{
			// Read equip information.
			bool cash = recv.readbool();
			int64_t uniqueid = -1;
			if (cash)
			{
				uniqueid = recv.readlong();
			}
			int64_t expire = recv.readlong();
			uint8_t slots = recv.readbyte();
			uint8_t level = recv.readbyte();

			// Read equip stats.
			using::Character::Equipstat;
			map<Equipstat, uint16_t> stats;
			for (Equipstat e = Character::ES_STR; e <= Character::ES_JUMP; e = (Equipstat)(e + 1))
			{
				stats[e] = recv.readshort();
			}

			// Some more information.
			string owner = recv.readascii();
			int16_t flag = recv.readshort();
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
				recv.readbyte();
				itemlevel = recv.readbyte();
				recv.readshort();
				itemexp = recv.readshort();
				vicious = recv.readint();
				recv.readlong();
			}

			recv.skip(12);

			inventory.addequip(invtype, slot, id, cash, uniqueid, expire, slots, 
				level, stats, owner, flag, itemlevel, itemexp, vicious);
		}
	};
}