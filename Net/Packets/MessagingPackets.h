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
#include "Net\OutPacket.h"
#include "Net\SendOpcodes.h"

namespace Net
{
	// Packet which sends a message to general chat.
	// Opcode: GENERAL_CHAT(49)
	class GeneralChatPacket : public OutPacket
	{
	public:
		GeneralChatPacket(string message, bool show) : OutPacket(GENERAL_CHAT)
		{
			writestr(message);
			writech(show);
		}
	};


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
	};

	class NpcTalkStringPacket : public NpcTalkMorePacket
	{
		NpcTalkStringPacket(string response) : NpcTalkMorePacket(2, 1)
		{
			writestr(response);
		}
	};

	class NpcTalkSelectionPacket : public NpcTalkMorePacket
	{
		NpcTalkSelectionPacket(int32_t selection) : NpcTalkMorePacket(4, 1)
		{
			writeint(selection);
		}
	};
}