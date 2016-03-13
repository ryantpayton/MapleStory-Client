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
#include "Net\PacketError.h"
#include "Util\Point.h"
#include <cstdint>
#include <functional>
#include <unordered_map>

namespace Net
{
	using std::function;
	using std::unordered_map;

	// A packet received from the server. 
	// Contains reading functions. 
	class InPacket
	{
	public:
		// Construct a packet from an array of bytes.
		InPacket(const int8_t*, size_t);

		// Check if there are more bytes available.
		bool hasmore() const;
		// Return the remaining length in bytes.
		size_t length() const;
		// Skip a number of bytes (by increasing the offset).
		void skip(size_t count);
		// Read a string.
		string readascii();
		// Read a fixed-length string.
		string readpadascii(int16_t length);
		// Read a point.
		Point<int16_t> readpoint();

		// Read a byte and check if it is 1.
		bool readbool() { return read<int8_t>() == 1; }
		// Read a byte.
		int8_t readbyte() { return read<int8_t>(); }
		// Read a short.
		int16_t readshort() { return read<int16_t>(); }
		// Read an int.
		int32_t readint() { return read<int32_t>(); }
		// Read a long.
		int64_t readlong() { return read<int64_t>(); }

		// Read an array into a buffer.
		template <class T>
		void readarray(T* buffer, size_t count)
		{
			for (size_t i = 0; i < count; i++)
			{
				buffer[i] = read<T>();
			}
		}

		// Read a pair.
		template <typename T1, typename T2>
		pair<T1, T2> read()
		{
			auto first = read<T1>();
			auto second = read<T2>();
			return std::make_pair(first, second);
		}

		// Read an unordered_map.
		template<typename K, typename V>
		unordered_map<K, V> readmap()
		{
			unordered_map<K, V> result;
			auto size = read<int16_t>();
			for (int16_t i = 0; i < size; i++)
			{
				auto key = read<K>();
				auto value = read<V>();
				result[key] = value;
			}
			return result;
		}

	private:
		template <typename T>
		T read()
		{
			size_t count = sizeof(T) / sizeof(int8_t);
			T all = 0;
			for (size_t i = 0; i < count; i++)
			{
				T val = static_cast<uint8_t>(bytes[pos]);
				all += val << (8 * i);
				skip(1);
			}
			return static_cast<T>(all);
		}

		const int8_t* bytes;
		size_t top;
		size_t pos;
	};
}

