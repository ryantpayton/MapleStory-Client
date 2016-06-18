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
#pragma once
#include "PacketError.h"

#include "..\Util\Point.h"

#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>
#include <queue>
#include <array>

namespace jrc
{
	// A packet received from the server. 
	// Contains reading functions. 
	class InPacket
	{
	public:
		// Construct a packet from an array of bytes.
		InPacket(const int8_t*, size_t);

		// Check if there are more bytes available.
		bool has_more() const;
		// Return the remaining length in bytes.
		size_t length() const;
		// Skip a number of bytes (by increasing the offset).
		void skip(size_t count);

		// Read a byte and check if it is 1.
		bool read_bool() { return read<bool>(); }
		// Read a byte.
		int8_t read_byte() { return read<int8_t>(); }
		// Read a short.
		int16_t read_short() { return read<int16_t>(); }
		// Read a int.
		int32_t read_int() { return read<int32_t>(); }
		// Read a long.
		int64_t read_long() { return read<int64_t>(); }

		// Read a point.
		Point<int16_t> read_point();

		// Read a string.
		std::string read_string();
		// Read a fixed-length string.
		std::string read_padded_string(uint16_t length);

		// Inspect a byte and check if it is 1.
		bool inspectbool() { return inspect<int8_t>() == 1; }
		// Inspect a byte.
		int8_t inspectbyte() { return inspect<int8_t>(); }
		// Inspect a short.
		int16_t inspectshort() { return inspect<int16_t>(); }
		// Inspect an int.
		int32_t inspectint() { return inspect<int32_t>(); }
		// Inspect a long.
		int64_t inspectlong() { return inspect<int64_t>(); }

		template <typename T>
		T read();

		template <>
		// Read a string. The size is specified by a short at the current position.
		std::string read<std::string>()
		{
			auto length = read<uint16_t>();
			return read_padded_string(length);
		}

		template <typename T1, typename T2>
		// Read a pair.
		std::pair<T1, T2> read()
		{
			auto first = read<T1>();
			auto second = read<T2>();
			return std::make_pair(first, second);
		}

		template<typename S, typename T>
		// Read a vector of type T. The length is read as a variable of type S.
		std::queue<T> readqueue()
		{
			std::queue<T> values;
			auto size = read<S>();
			for (S i = 0; i < size; i++)
			{
				auto value = read<T>();
				values.push(value);
			}
			return values;
		}

		template<typename S, typename T>
		// Read a vector of type T. The length is read as a variable of type S.
		std::vector<T> readvector()
		{
			std::vector<T> values;
			auto size = read<S>();
			for (S i = 0; i < size; i++)
			{
				auto value = read<T>();
				values.push_back(value);
			}
			return values;
		}

		template<typename K, typename V>
		// Read an unordered_map.
		std::unordered_map<K, V> readmap()
		{
			std::unordered_map<K, V> result;
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
		template <>
		// Read a byte and check if it is 1.
		bool read<bool>() { return readnumber<int8_t>() == 1; }

		template <>
		// Read a byte.
		int8_t read<int8_t>() { return readnumber<int8_t>(); }
		template <>
		// Read an unsigned byte.
		uint8_t read<uint8_t>() { return readnumber<int8_t>(); }

		template <>
		// Read a short.
		int16_t read<int16_t>() { return readnumber<int16_t>(); }
		template <>
		// Read an unsigned short.
		uint16_t read<uint16_t>() { return readnumber<uint16_t>(); }

		template <>
		// Read an int.
		int32_t read<int32_t>() { return readnumber<int32_t>(); }
		template <>
		// Read an unsigned int.
		uint32_t read<uint32_t>() { return readnumber<uint32_t>(); }

		template <>
		// Read a long.
		int64_t read<int64_t>() { return readnumber<int64_t>(); }
		template <>
		// Read an unsigned long.
		uint64_t read<uint64_t>() { return readnumber<uint64_t>(); }

		template <typename T>
		// Read a number and advance the buffer position.
		T readnumber()
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

		template <class T>
		// Read without advancing the buffer position.
		T inspect()
		{
			size_t before = pos;
			T value = read<T>();
			pos = before;
			return value;
		}

		const int8_t* bytes;
		size_t top;
		size_t pos;
	};
}

