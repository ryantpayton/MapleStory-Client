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
#include "Util\Singleton.h"
#include "OutPacket.h"
#include "Login.h"
#include "Cryptography.h"
#include "PacketSwitch.h"

#include "Journey.h"
#ifdef JOURNEY_USE_ASIO
#include "SocketAsio.h"
#else
#include "SocketWinsock.h"
#endif

namespace Net
{
	class Session : public Singleton<Session>
	{
	public:
		Session();
		~Session();

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

	private:
		bool init(const char* host, const char* port);
		void process(const int8_t* bytes, size_t available);

		static const size_t MIN_PACKET_LEN = HEADERLEN + 2;

		const Cryptography crypto;
		const PacketSwitch packetswitch;

		Login login;

		bool connected;

		int8_t buffer[MAX_PACKET_LEN];
		size_t length;
		size_t pos;

#ifdef JOURNEY_USE_ASIO
		SocketAsio socket;
#else
		SocketWinsock socket;
#endif

#ifdef JOURNEY_USE_CRYPTO
		uint8_t sendiv[HEADERLEN];
		uint8_t recviv[HEADERLEN];
#endif
	};
}

