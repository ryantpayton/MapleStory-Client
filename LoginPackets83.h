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
#include "OutPacket.h"
#include "SendOpcodes83.h"

namespace Net
{
	class TOSPacket83 : public OutPacket
	{
	public:
		TOSPacket83() : OutPacket(ACCEPT_TOS)
		{
			writech(1);
		}
	};

	class LoginPacket83 : public OutPacket
	{
	public:
		LoginPacket83(string acc, string pass) : OutPacket(LOGIN)
		{
			writestr(acc);
			writestr(pass);
		}
	};

	class ServerRequestPacket83 : public OutPacket
	{
	public:
		ServerRequestPacket83() : OutPacket(SERVERLIST_REQUEST) {}
	};

	class CharlistRequestPacket83 : public OutPacket
	{
	public:
		CharlistRequestPacket83(uint8_t world, uint8_t channel) : OutPacket(CHARLIST_REQUEST)
		{
			writech(0);
			writech(world);
			writech(channel);
		}
	};

	class PlayerLoginPacket83 : public OutPacket
	{
	public:
		PlayerLoginPacket83(int cid) : OutPacket(PLAYER_LOGIN)
		{
			writeint(cid);
		}
	};
}