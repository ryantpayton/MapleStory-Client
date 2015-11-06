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
#include "PacketError.h"
#include "vector2d.h"
#include <cstdint>
#include <string>

namespace Net
{
	using::std::int8_t;
	using::std::int16_t;
	using::std::int32_t;
	using::std::int64_t;
	using::std::string;
	using::Util::vector2d;
	// A packet sent by the server. Contains read functions used by handlers to retrieve meaningful information. 
	class InPacket
	{
	public:
		InPacket(const int8_t*, size_t);
		size_t length() const;
		void skip(size_t);
		string readascii();
		string readpadascii(int16_t);
		vector2d<int16_t> readpoint();
		bool readbool() { return read<int8_t>() == 1; }
		int8_t readbyte() { return read<int8_t>(); }
		int16_t readshort() { return read<int16_t>(); }
		int32_t readint() { return read<int32_t>(); }
		int64_t readlong() { return read<int64_t>(); }

		template <class T>
		void readarray(T* buffer, size_t count)
		{
			for (size_t i = 0; i < count; i++)
			{
				buffer[i] = read<T>();
			}
		}

	private:
		template <class T>
		T read()
		{
			size_t count = sizeof(T) / sizeof(int8_t);
			if (count > top - pos)
				throw PacketError("Stack underflow while reading.");

			size_t all = 0;
			for (size_t i = 0; i < count; i++)
			{
				all += static_cast<uint8_t>(bytes[pos]) * static_cast<size_t>(pow(256, i));
				pos++;
			}
			return static_cast<T>(all);
		}

		const int8_t* bytes;
		size_t top;
		size_t pos;
	};
}

