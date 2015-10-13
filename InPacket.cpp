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
#include "InPacket.h"

namespace Net
{
	InPacket::InPacket(const char* recv, int32_t length)
	{
		writearray(recv, length);
	}

	void InPacket::skip(size_t count)
	{
		bytes.resize(bytes.size() - count);
	}

	string InPacket::readascii()
	{
		int16_t length = read<int16_t>();
		return readpadascii(length);
	}

	string InPacket::readpadascii(int16_t length)
	{
		string ret;

		if (length > bytes.size())
		{
			throw new runtime_error("packet error: stack underflow");
		}
		else
		{
			for (int16_t i = 0; i < length; i++)
			{
				char letter = bytes.back();
				if (letter != '\0')
				{
					ret.push_back(letter);
				}
				bytes.pop_back();
			}
		}

		return ret;
	}

	vector2d<int16_t> InPacket::readpoint()
	{
		int16_t x = read<int16_t>();
		int16_t y = read<int16_t>();
		return vector2d<int16_t>(x, y);
	}
}
