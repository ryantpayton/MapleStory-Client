//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2016 Daniel Allendorf                                        //
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
#include "..\OutPacket.h"
#include "..\SendOpcodes.h"

namespace jrc
{
	// Packet which requests a dialogue with a server-sided npc.
	// Opcode: TALK_TO_NPC(58)
	class TalkToNPCPacket : public OutPacket
	{
	public:
		TalkToNPCPacket(int32_t oid) : OutPacket(TALK_TO_NPC)
		{
			writeint(oid);
		}
	};

	// Packet which sends a response to an npc dialogue to the server.
	// Opcode: NPC_TALK_MORE(60)
	class NpcTalkMorePacket : public OutPacket
	{
	public:
		NpcTalkMorePacket(int8_t lastmsg, int8_t response) : OutPacket(NPC_TALK_MORE)
		{
			writech(lastmsg);
			writech(response);
		}

		NpcTalkMorePacket(const std::string& response) : NpcTalkMorePacket(2, 1)
		{
			writestr(response);
		}

		NpcTalkMorePacket(int32_t selection) : NpcTalkMorePacket(4, 1)
		{
			writeint(selection);
		}
	};


	// Packet which tells the server of an interaction with an npc shop.
	// Opcode: NPC_SHOP_ACTION(61)
	class NpcShopActionPacket : public OutPacket
	{
	public:
		// Requests that an item should be bought from or sold to a npc shop.
		NpcShopActionPacket(int16_t slot, int32_t itemid, int16_t qty, bool buy) 
			: NpcShopActionPacket(buy ? BUY : SELL) {

			writesh(slot);
			writeint(itemid);
			writesh(qty);
		}

		// Requests that an item should be recharged at a npc shop.
		NpcShopActionPacket(int16_t slot) 
			: NpcShopActionPacket(RECHARGE) {

			writesh(slot);
		}

		// Requests exiting from a npc shop.
		NpcShopActionPacket() 
			: NpcShopActionPacket(LEAVE) {}

	protected:
		enum Mode : int8_t
		{
			BUY, SELL, RECHARGE, LEAVE
		};

		NpcShopActionPacket(Mode mode) : OutPacket(NPC_SHOP_ACTION)
		{
			writech(mode);
		}
	};
}