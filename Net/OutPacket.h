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
#include <stdint.h>
#include <string>
#include <vector>

namespace Net
{
	using::std::string;
	using::std::vector;
	// A packet to be sent by the client. Used as a base class to create specific packets.
	class OutPacket
	{
	public:
		OutPacket(int16_t);
		size_t length() const;
		const int8_t* getbytes() const;

	protected:
		void skip(size_t);
		void writestr(string);
		void writech(int8_t c) { bytes.push_back(c); }
		void writesh(int16_t s) { write<int16_t>(s); }
		void writeint(int32_t i) { write<int32_t>(i); }
		void writelg(int64_t l) { write<int64_t>(l); }

	private:
		template <class T>
		void write(T num)
		{
			size_t size = sizeof(T) / sizeof(int8_t);
			for (size_t i = 0; i < size; i++)
			{
				bytes.push_back(static_cast<int8_t>(num));
				num = num >> 8;
			}
		}

		vector<int8_t> bytes;
	};
}

