/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "Account.h"
#include "World.h"

namespace Net
{
	class Login
	{
	public:
		Login() {}
		~Login() {}
		void parseaccount(InPacket&);
		void parseworld(InPacket&);
		void setworldid(uint8_t w) { worldid = w; }
		void setchannelid(uint8_t c) { channelid = c; }
		void setcharid(int c) { charid = c; }
		const World& getworld(size_t w) { return worlds[w]; }
		uint8_t getworldid() { return worldid; }
		uint8_t getchannelid() { return channelid; }
		int getcharid() { return charid; }
		Account& getaccount() { return account; }
	private:
		Account account;
		vector<World> worlds;
		uint8_t worldid;
		uint8_t channelid;
		int charid;
	};
}