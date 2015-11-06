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
#include "SessionServer.h"

namespace Net
{
	// Default values. Connect to a localhost server's login port.
	const string HOST = "127.0.0.1";
	const string PORT = "8484";

	SessionServer::SessionServer()
	{
		connected = false;
	}

	SessionServer::~SessionServer() {}

	bool SessionServer::init()
	{
		// Connect with default values.
		return init(HOST.c_str(), PORT.c_str());
	}

	bool SessionServer::init(const char* host, const char* port)
	{
		// Connect to the server and attempt to read the handshake packet.
		connected = socket.open(host, port);
		if (connected)
		{
			InPacket handshake = InPacket(socket.getbuffer(), HANDSHAKE_LEN);
			handshake.skip(7);
			// Read in iv's neccessary for communicating with the server.
			handshake.readarray<uint8_t>(sendiv, 4);
			handshake.readarray<uint8_t>(recviv, 4);
		}
		// Return if connected successfully.
		return connected;
	}

	bool SessionServer::receive(ClientInterface& client)
	{
		// Check if a packet has arrived. Handle if data is sufficient: 4 bytes(header) + 2 bytes(opcode) = 6.
		size_t result = socket.receive(&connected);
		if (result > 5)
		{
			// Retrieve buffer from the socket and process it.
			const int8_t* bytes = socket.getbuffer();
			process(client, bytes, result);
		}
		// Return if the connection is still alive.
		return connected;
	}

	void SessionServer::process(ClientInterface& client, const int8_t* bytes, size_t available)
	{
		if (pos == 0)
		{
			// Pos is 0, meaning this is the start of a new packet. Start by determining length.
			length = crypto.getlength(bytes);
			// Reading the length means we processed the header. Move forward by 4.
			bytes = bytes + 4;
			available -= 4;
		}

		// Determine how much we can write. Write data into the buffer.
		size_t towrite = length - pos;
		if (towrite > available)
		{
			towrite = available;
		}
		memcpy(buffer + pos, bytes, towrite);
		pos += available;

		// Check if the current packet has been fully processed.
		if (pos >= length)
		{
			// Create InPacket from the buffer, decrypt it and pass it on to the PacketHandler.
			InPacket recv = InPacket(buffer, length);
			crypto.decrypt(buffer, length, recviv);
			phandler.handle(client, recv);
			// Rewind our position by length processed and check if there is more available.
			pos -= length;
			if (pos > 0)
			{
				pos = 0;
				// More packets are available, so we start over. Recursion is safe as this only occurs for 3 or 4 times max.
				process(client, bytes + length, available - length);
			}
		}
	}

	void SessionServer::dispatch(const OutPacket& tosend)
	{
		// The packet 'tosend' arrives without header so total length is + 4.
		size_t total = tosend.length() + 4;
		// Create a temporary buffer and copy packet's bytes.
		int8_t* bytes = new int8_t[total];
		memcpy(bytes + 4, tosend.getbytes(), tosend.length());
		// Add the header and encrypt the data.
		crypto.encrypt(bytes, tosend.length(), sendiv);
		// Send packet and delete buffer.
		socket.dispatch(bytes, total);
		delete[] bytes;
	}

	bool SessionServer::reconnect(const char* address, const char* port)
	{
		// Close the current connection and open a new one.
		bool error = socket.close();
		return error ? init(address, port) : false;
	}

	void SessionServer::disconnect()
	{
		// The next call to receive will return false, this is assumed to quit the program.
		connected = false;
	}
}
