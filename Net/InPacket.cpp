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
#include "InPacket.h"

namespace jrc
{
	InPacket::InPacket(const int8_t* recv, size_t length)
	{
		bytes = recv;
		top = length;
		pos = 0;
	}

	bool InPacket::hasmore() const
	{
		return length() > 0;
	}

	size_t InPacket::length() const
	{
		return top - pos;
	}

	void InPacket::skip(size_t count)
	{
		if (count > length())
			throw PacketError("Stack underflow at " + std::to_string(pos));

		pos += count;
	}

	std::string InPacket::read_padded_string(uint16_t count)
	{
		std::string ret;
		for (int16_t i = 0; i < count; i++)
		{
			char letter = read_byte();
			if (letter != '\0')
			{
				ret.push_back(letter);
			}
		}
		return ret;
	}

	std::string InPacket::read_string()
	{
		return read<std::string>();
	}

	Point<int16_t> InPacket::readpoint()
	{
		auto x = read<int16_t>();
		auto y = read<int16_t>();
		return Point<int16_t>(x, y);
	}
}
