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
#ifdef JOURNEY_USE_ASIO

#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB
#include "asio.hpp"

namespace Net
{
	// MAX_PACKET_LEN already had to be changed 3 times.
	const size_t MAX_PACKET_LEN = 40960;
	const size_t HANDSHAKE_LEN = 16;

	using::asio::io_service;
	using::asio::ip::tcp;
	using::asio::error_code;
	// Class that wraps an asio socket.
	class SocketAsio
	{
	public:
		SocketAsio();
		~SocketAsio();
		bool open(const char*, const char*);
		bool close();
		size_t receive(bool*);
		const int8_t* getbuffer() const;
		bool dispatch(const int8_t*, size_t);
	private:
		io_service ioservice;
		tcp::resolver resolver;
		tcp::socket socket;
		int8_t buffer[MAX_PACKET_LEN];
	};
}
#endif
