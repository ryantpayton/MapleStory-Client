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
	//Contains methods for communicating with the server.
	namespace Session
	{
		// Connect to the server using the default values for adress and login port, return if successfull.
		bool init();
		// Closes the current connection and opens a new one.
		bool reconnect(const char* adress, const char* port);
		// The next call to receive() will return false, qutting the game.
		void disconnect();
		// Check for incoming packets and handle them. Returns if the connection is still alive.
		bool receive();
		// Sends a packet to the server, using the cryptography to encrypt data and add the header.
		void dispatch(const OutPacket& tosend);
		// Obtain a reference to the login information.
		Login& getlogin();
	}
}

