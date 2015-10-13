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
#pragma once
#include <string>
#include "Packet.h"
#include "vector2d.h"

using namespace Util;

namespace Net
{
	class InPacket : public Packet
	{
	public:
		InPacket(const char*, int32_t);
		InPacket() {}
		~InPacket() {}
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
		vector<T> readvector(size_t count)
		{
			vector<T> ret;
			for (size_t i = 0; i < count; i++)
			{
				ret.push_back(read<T>());
			}
			return ret;
		}
	private:
		template <class T>
		T read()
		{
			size_t size = sizeof(T) / sizeof(int8_t);
			if (bytes.size() < size)
			{
				throw new runtime_error("packet error: stack underflow");
			}
			else
			{
				size_t all = 0;
				for (size_t i = 0; i < size; i++)
				{
					all += static_cast<uint8_t>(bytes.back()) * static_cast<size_t>(pow(256, i));
					bytes.pop_back();
				}
				return static_cast<T>(all);
			}
		}
	};
}

