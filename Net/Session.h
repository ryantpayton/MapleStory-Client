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
#include "SessionInterface.h"
#include "Cryptography.h"
#include "PacketHandler83.h"
#include "Program\ClientInterface.h"

#include "Journey.h"
#ifdef JOURNEY_USE_ASIO
#include "SocketAsio.h"
#else
#include "SocketWinsock.h"
#endif

namespace Net
{
	// Class used to communicate with the server.
	class Session : public SessionInterface
	{
	public:
		Session();
		~Session();

		// Connect to the server using the default values for adress and login port, return if successfull.
		bool init();
		// Connect to the server using the specified values, return if successfull.
		bool init(const char* adress, const char* port);
		// Check for incoming packets and handle them. Returns if the connection is still alive.
		bool receive(ClientInterface& client);

		// Sends a packet to the server, using the cryptography to encrypt data and add the header.
		void dispatch(const OutPacket& tosend) override;
		// Closes the current connection and opens a new one.
		bool reconnect(const char* adress, const char* port) override;
		// The next call to receive() will return false, qutting the game.
		void disconnect() override;
		// Return the object with the current login information.
		Login& getlogin() override;

	private:
		void process(ClientInterface&, const int8_t*, size_t);

#ifdef JOURNEY_USE_ASIO
		SocketAsio socket;
#else
		SocketWinsock socket;
#endif

		Cryptography crypto;
		PacketHandler83 phandler;

		Login login;

		bool connected;
		uint8_t sendiv[4];
		uint8_t recviv[4];
		int8_t buffer[MAX_PACKET_LEN];
		size_t length;
		size_t pos;
	};
}

