//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "InPacket.h"

namespace ms
{
	InPacket::InPacket(const int8_t* recv, size_t length)
	{
		bytes = recv;
		top = length;
		pos = 0;
	}

	bool InPacket::available() const
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

	bool InPacket::read_bool()
	{
		return read_byte() == 1;
	}

	int8_t InPacket::read_byte()
	{
		return read<int8_t>();
	}

	int16_t InPacket::read_short()
	{
		return read<int16_t>();
	}

	int32_t InPacket::read_int()
	{
		return read<int32_t>();
	}

	int64_t InPacket::read_long()
	{
		return read<int64_t>();
	}

	Point<int16_t> InPacket::read_point()
	{
		auto x = read<int16_t>();
		auto y = read<int16_t>();

		return { x, y };
	}

	std::string InPacket::read_string()
	{
		auto length = read<uint16_t>();

		return read_padded_string(length);
	}

	std::string InPacket::read_padded_string(uint16_t count)
	{
		std::string ret;

		for (int16_t i = 0; i < count; i++)
		{
			char letter = read_byte();

			if (letter != '\0')
				ret.push_back(letter);
		}

		return ret;
	}

	bool InPacket::inspect_bool()
	{
		return inspect_byte() == 1;
	}

	int8_t InPacket::inspect_byte()
	{
		return inspect<int8_t>();
	}

	int16_t InPacket::inspect_short()
	{
		return inspect<int16_t>();
	}

	int32_t InPacket::inspect_int()
	{
		return inspect<int32_t>();
	}

	int64_t InPacket::inspect_long()
	{
		return inspect<int64_t>();
	}
}