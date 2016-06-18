/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "Login.h"
#include "Cryptography.h"
#include "PacketSwitch.h"

#include "..\Error.h"
#include "..\Util\Singleton.h"

#include "..\Journey.h"
#ifdef JOURNEY_USE_ASIO
#include "SocketAsio.h"
#else
#include "SocketWinsock.h"
#endif

namespace jrc
{
	class Session : public Singleton<Session>
	{
	public:
		Session();
		~Session();

		// Connect to the server using the default values for adress and login port, return if successfull.
		Error init();
		// Closes the current connection and opens a new one.
		void reconnect(const char* adress, const char* port);
		// Mark the session as disconnected.
		void disconnect();
		// Check for incoming packets and handle them. Returns if the connection is still alive.
		bool receive();

		// Obtain a reference to the login information.
		Login& get_login();

	private:
		void dispatch(int8_t* bytes, size_t length);
		friend void OutPacket::dispatch();

		void process(const int8_t* bytes, size_t available);
		bool init(const char* host, const char* port);

		PacketSwitch packetswitch;
		Cryptography cryptography;
		Login login;

		int8_t buffer[MAX_PACKET_LENGTH];
		size_t length;
		size_t pos;
		bool connected;

#ifdef JOURNEY_USE_ASIO
		SocketAsio socket;
#else
		SocketWinsock socket;
#endif
	};
}

