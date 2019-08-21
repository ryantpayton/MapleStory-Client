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
#include "../Configuration.h"

namespace ms
{
	// Send user selected Gender
	// Opcode: SET_GENDER(8)
	class GenderPacket : public OutPacket
	{
	public:
		GenderPacket(const bool female) : OutPacket(OutPacket::Opcode::SET_GENDER)
		{
			write_byte(1);
			write_byte(female);
		}
	};

	// Accept the Terms of Service.
	// Opcode: ACCEPT_TOS(7)
	class TOSPacket : public OutPacket
	{
	public:
		TOSPacket() : OutPacket(OutPacket::Opcode::ACCEPT_TOS)
		{
			write_byte(1);
		}
	};

	// Request to be logged-in to an account.
	// Opcode: LOGIN(1)
	class LoginPacket : public OutPacket
	{
	public:
		LoginPacket(const std::string& acc, const std::string& pass) : OutPacket(OutPacket::Opcode::LOGIN)
		{
			std::string volumeSerialNumber = Configuration::get().get_vol_serial_num();

			std::string part1 = volumeSerialNumber.substr(0, 2);
			std::string part2 = volumeSerialNumber.substr(2, 2);
			std::string part3 = volumeSerialNumber.substr(4, 2);
			std::string part4 = volumeSerialNumber.substr(6, 2);

			const int32_t h = hex_to_dec(part4);
			const int32_t w = hex_to_dec(part3);
			const int32_t i = hex_to_dec(part2);
			const int32_t d = hex_to_dec(part1);

			write_string(acc);
			write_string(pass);
			skip(6);
			write_byte(h);
			write_byte(w);
			write_byte(i);
			write_byte(d);
		}
	};

	// Requests the list of worlds and channels.
	// Opcode: SERVERLIST_REQUEST(11)
	class ServerRequestPacket : public OutPacket
	{
	public:
		ServerRequestPacket() : OutPacket(OutPacket::Opcode::SERVERLIST_REQUEST) {}
	};

	// Requests the list of characters on a world.
	// Opcode: CHARLIST_REQUEST(5)
	class CharlistRequestPacket : public OutPacket
	{
	public:
		CharlistRequestPacket(uint8_t world, uint8_t channel) : OutPacket(OutPacket::Opcode::CHARLIST_REQUEST)
		{
			write_byte(0);
			write_byte(world);
			write_byte(channel);
		}
	};

	// Requests being logged-in to a channel server with the specified character.
	// Opcode: PLAYER_LOGIN(20)
	class PlayerLoginPacket : public OutPacket
	{
	public:
		PlayerLoginPacket(int32_t cid) : OutPacket(OutPacket::Opcode::PLAYER_LOGIN)
		{
			write_int(cid);
		}
	};
}