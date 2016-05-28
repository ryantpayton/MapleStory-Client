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
#include "..\OutPacket.h"
#include "..\SendOpcodes.h"

namespace jrc
{
	// Accept the Terms of Service.
	// Opcode: ACCEPT_TOS(7)
	class TOSPacket : public OutPacket
	{
	public:
		TOSPacket() : OutPacket(ACCEPT_TOS)
		{
			writech(1);
		}
	};

	// Request to be logged-in to an account.
	// Opcode: LOGIN(1)
	class LoginPacket : public OutPacket
	{
	public:
		LoginPacket(const std::string& acc, const std::string& pass) : OutPacket(LOGIN)
		{
			writestr(acc);
			writestr(pass);
		}
	};

	// Requests the list of worlds and channels.
	// Opcode: SERVERLIST_REQUEST(11)
	class ServerRequestPacket : public OutPacket
	{
	public:
		ServerRequestPacket() : OutPacket(SERVERLIST_REQUEST) {}
	};

	// Requests the list of characters on a world.
	// Opcode: CHARLIST_REQUEST(5)
	class CharlistRequestPacket : public OutPacket
	{
	public:
		CharlistRequestPacket(uint8_t world, uint8_t channel) : OutPacket(CHARLIST_REQUEST)
		{
			writech(0);
			writech(world);
			writech(channel);
		}
	};

	// Requests being logged-in to a channel server with the specified character.
	// Opcode: PLAYER_LOGIN(20)
	class PlayerLoginPacket : public OutPacket
	{
	public:
		PlayerLoginPacket(int32_t cid) : OutPacket(PLAYER_LOGIN)
		{
			writeint(cid);
		}
	};
}
