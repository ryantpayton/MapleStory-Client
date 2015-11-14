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
#include "Journey.h"
#ifndef JOURNEY_USE_ASIO
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <cstdint>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace Net
{
	const size_t MAX_PACKET_LEN = 40960;
	const size_t HANDSHAKE_LEN = 16;

	class SocketWinsock
	{
	public:
		SocketWinsock();
		~SocketWinsock();

		bool open(const char* adress, const char* port);
		bool close();
		size_t receive(bool* connected);
		const int8_t* getbuffer();
		bool dispatch(const int8_t* bytes, size_t length);

	private:
		SOCKET sock;
		int8_t buffer[MAX_PACKET_LEN];
	};
}
#endif

