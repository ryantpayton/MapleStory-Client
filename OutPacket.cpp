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
#include "OutPacket.h"

namespace Net
{
	OutPacket::OutPacket(int16_t opcode)
	{
		write<int16_t>(opcode);
	}

	void OutPacket::skip(size_t count)
	{
		for (size_t i = 0; i < count; i++)
		{
			bytes.push_back(0);
		}
	}

	void OutPacket::writeheader(char* recv)
	{
		for (int8_t i = 3; i >= 0; i--)
		{
			bytes.insert(bytes.begin(), recv[i]);
		}
	}

	void OutPacket::writestr(string str)
	{
		int16_t len = static_cast<int16_t>(str.length());
		write<int16_t>(len);
		for (int16_t i = 0; i < len; i++)
		{
			bytes.push_back(str[i]);
		}
	}
}
