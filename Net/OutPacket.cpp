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
#include "OutPacket.h"

#include "Session.h"

#include <chrono>

namespace jrc
{
	OutPacket::OutPacket(int16_t opcode)
	{
		writesh(opcode);
	}

	void OutPacket::dispatch()
	{
		Session::get()
			.dispatch(bytes.data(), bytes.size());
	}

	void OutPacket::skip(size_t count)
	{
		for (size_t i = 0; i < count; i++)
		{
			bytes.push_back(0);
		}
	}

	void OutPacket::writech(int8_t ch)
	{
		bytes.push_back(ch);
	}

	void OutPacket::writesh(int16_t sh)
	{
		for (size_t i = 0; i < 2; i++)
		{
			writech(static_cast<int8_t>(sh));
			sh >>= 8;
		}
	}

	void OutPacket::writeint(int32_t in)
	{
		for (size_t i = 0; i < 4; i++)
		{
			writech(static_cast<int8_t>(in));
			in >>= 8;
		}
	}

	void OutPacket::writelg(int64_t lg)
	{
		for (size_t i = 0; i < 8; i++)
		{
			writech(static_cast<int8_t>(lg));
			lg >>= 8;
		}
	}

	void OutPacket::writetime()
	{
		auto duration = std::chrono::steady_clock::now().time_since_epoch();
		auto since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		auto timestamp = static_cast<int32_t>(since_epoch.count());
		writeint(timestamp);
	}

	void OutPacket::writepoint(Point<int16_t> position)
	{
		writesh(position.x());
		writesh(position.y());
	}

	void OutPacket::writestr(const std::string& str)
	{
		int16_t length = static_cast<int16_t>(str.length());
		writesh(length);

		for (int16_t i = 0; i < length; i++)
		{
			writech(str[i]);
		}
	}
}
