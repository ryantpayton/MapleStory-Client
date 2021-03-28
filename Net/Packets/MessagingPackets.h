//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../OutPacket.h"

namespace ms
{
	// Packet which sends a message to general chat
	// Opcode: GENERAL_CHAT(49)
	class GeneralChatPacket : public OutPacket
	{
	public:
		GeneralChatPacket(const std::string& message, bool show) : OutPacket(OutPacket::Opcode::GENERAL_CHAT)
		{
			write_string(message);
			write_byte(show);
		}
	};

	// Packet which sends a message to different chats
	// Opcode: MULTI_CHAT(119)
	class MultiChatPacket : public OutPacket
	{
	public:
		enum Type : uint8_t
		{
			BUDDY,
			PARTY,
			GUILD,
			ALLIANCE
		};

		MultiChatPacket(Type type, const std::list<int32_t>& recipients, const std::string& message) : OutPacket(OutPacket::Opcode::MULTI_CHAT)
		{
			write_byte(type);
			write_byte(recipients.size());

			for (int32_t recipient : recipients)
				write_int(recipient);

			write_string(message);
		}
	};

	// Packet which sends a message to a spouse
	// Opcode: SPOUSE_CHAT(121)
	class SpouseChatPacket : public OutPacket
	{
	public:
		SpouseChatPacket(const std::string& message) : OutPacket(OutPacket::Opcode::SPOUSE_CHAT)
		{
			write_string(message);
		}
	};
}