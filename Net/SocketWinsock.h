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
#include <cstdlib>
#include <cstdint>

namespace Net
{
	const size_t MAX_PACKET_LEN = 131072;

#ifdef JOURNEY_USE_CRYPTO
	const size_t HANDSHAKE_LEN = 16;
#else
	const size_t HANDSHAKE_LEN = 2;
#endif

	class SocketWinsock
	{
	public:
		bool open(const char* adress, const char* port);
		bool close();
		size_t receive(bool* connected);
		const int8_t* getbuffer();
		bool dispatch(const int8_t* bytes, size_t length);

	private:
		uint64_t sock;
		int8_t buffer[MAX_PACKET_LEN];
	};
}
#endif

