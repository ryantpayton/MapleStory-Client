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
#include "OutPacket.h"
#include <chrono>

namespace Net
{
	OutPacket::OutPacket(int16_t opcode)
	{
		writesh(opcode);
	}

	size_t OutPacket::length() const
	{
		return bytes.size();
	}

	const int8_t* OutPacket::getbytes() const
	{
		return bytes.data();
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
			bytes.push_back(static_cast<int8_t>(sh));
			sh = sh >> 8;
		}
	}

	void OutPacket::writeint(int32_t in)
	{
		for (size_t i = 0; i < 4; i++)
		{
			bytes.push_back(static_cast<int8_t>(in));
			in = in >> 8;
		}
	}

	void OutPacket::writelg(int64_t lg)
	{
		for (size_t i = 0; i < 8; i++)
		{
			bytes.push_back(static_cast<int8_t>(lg));
			lg = lg >> 8;
		}
	}

	void OutPacket::writestr(string str)
	{
		int16_t len = static_cast<int16_t>(str.length());
		writesh(len);
		for (int16_t i = 0; i < len; i++)
		{
			bytes.push_back(str[i]);
		}
	}

	void OutPacket::writepoint(vector2d<int16_t> position)
	{
		writesh(position.x());
		writesh(position.y());
	}

	void OutPacket::writetime()
	{
		using std::chrono::steady_clock;
		using std::chrono::seconds;

		seconds since_epoch = std::chrono::duration_cast<seconds>(steady_clock::now().time_since_epoch());
		int32_t timestamp = static_cast<int32_t>(since_epoch.count());
		writeint(timestamp);
	}
}
