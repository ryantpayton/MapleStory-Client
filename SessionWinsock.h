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
#include "Cryptography.h"
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <mutex>
#include "ParentHandler.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace Net
{
	const string HOST = "localhost";
	const string LOGINPORT = "8484";
	const size_t BUFFERLEN = 10240;

	class SessionWinsock : public Session
	{
	public:
		SessionWinsock();
		~SessionWinsock();
		bool init(ParentHandler*);
		void open(const char*, const char*);
		void close();
		bool receive();
		void dispatch(OutPacket&);
		void reconnect(vector<uint8_t>, int16_t);
		void disconnect() { connected = false; }
		bool isconnected() { return connected; }
	private:
		void process(char*, int);
		Cryptography* crypto;
		ParentHandler* handler;
		SOCKET sock;
		char buffer[BUFFERLEN];
		InPacket toread;
		mutex sendlock;
		mutex recvlock;
		int bufferpos;
		short p_length;
		bool active;
		bool connected;
	};
}

