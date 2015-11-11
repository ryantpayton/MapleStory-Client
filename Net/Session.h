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
#include "Login\Login.h"

namespace Net
{
	// Interface for class that is used to communicate with the server.
	class Session
	{
	public:
		virtual ~Session() {}
		// Send a packet to the server. This will be a specific packet class that inerits from OutPacket.
		virtual void dispatch(const OutPacket&) = 0;
		// Used for login transition or changing channels.
		virtual bool reconnect(const char*, const char*) = 0;
		// Disconnect from the server, effectively terminating the program. Used to quit the game.
		virtual void disconnect() = 0;
		// Return the object with the current login information.
		virtual Login& getlogin() = 0;
	};
}