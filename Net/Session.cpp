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
#include "Session.h"
#include "Configuration.h"
#include "Console.h"

namespace Net
{
	Session::Session()
	{
		connected = false;
		length = 0;
		pos = 0;
	}

	Session::~Session() 
	{
		socket.close();
	}

	bool Session::init(const char* host, const char* port)
	{
		// Connect to the server and attempt to read the handshake packet.
		connected = socket.open(host, port);
#ifdef JOURNEY_USE_CRYPTO
		if (connected)
		{
			InPacket handshake = InPacket(socket.getbuffer(), HANDSHAKE_LEN);
			handshake.skip(7);
			// Read keys neccessary for communicating with the server.
			handshake.readarray<uint8_t>(sendiv, 4);
			handshake.readarray<uint8_t>(recviv, 4);
		}
#endif
		return connected;
	}

	bool Session::init()
	{
		string HOST = Configuration::get().getsetting(Settings::SERVER_IP);
		string PORT = "8484";
		return init(HOST.c_str(), PORT.c_str());
	}

	bool Session::reconnect(const char* address, const char* port)
	{
		// Close the current connection and open a new one.
		bool error = socket.close();
		return error ? init(address, port) : false;
	}

	void Session::disconnect()
	{
		connected = false;
	}

	void Session::process(const int8_t* bytes, size_t available)
	{
		if (pos == 0)
		{
			// Pos is 0, meaning this is the start of a new packet. Start by determining length.
			length = crypto.getlength(bytes);
			// Reading the length means we processed the header. Move forward by the header length.
			bytes = bytes + HEADERLEN;
			available -= HEADERLEN;
		}

		// Determine how much we can write. Write data into the buffer.
		size_t towrite = length - pos;
		if (towrite > available)
			towrite = available;
		memcpy(buffer + pos, bytes, towrite);
		pos += towrite;

		// Check if the current packet has been fully processed.
		if (pos >= length)
		{

#ifdef JOURNEY_USE_CRYPTO
			crypto.decrypt(buffer, length, recviv);
#endif
			try 
			{
				packetswitch.forward(buffer, length);
			}
			catch (const PacketError& err)
			{
				Console::get().print("Packet Error: " + string(err.what()));
			}

			pos = 0;
			length = 0;

			// Check if there is more available.
			size_t remaining = available - towrite;
			if (remaining >= MIN_PACKET_LEN)
			{
				// More packets are available, so we start over.
				process(bytes + towrite, remaining);
			}
		}
	}

	bool Session::receive()
	{
		// Check if a packet has arrived. Handle if data is sufficient: 4 bytes(header) + 2 bytes(opcode) = 6.
		size_t result = socket.receive(&connected);
		if (result >= MIN_PACKET_LEN || length > 0)
		{
			// Retrieve buffer from the socket and process it.
			const int8_t* bytes = socket.getbuffer();
			process(bytes, result);
		}
		// Return if the connection is still alive.
		return connected;
	}

	void Session::dispatch(const OutPacket& tosend)
	{
#ifdef JOURNEY_USE_CRYPTO
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
#else
		int8_t header[HEADERLEN];
		crypto.getheader(header, tosend.length());
		socket.dispatch(header, HEADERLEN);
		socket.dispatch(tosend.getbytes(), tosend.length());
#endif
	}

	Login& Session::getlogin()
	{
		return login;
	}
}
