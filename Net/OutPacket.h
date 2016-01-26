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
#include "Util\vector2d.h"
#include <cstdint>
#include <string>
#include <vector>
#include <map>

namespace Net
{
	using std::string;
	using std::vector;
	using std::map;

	// A packet to be sent to the server. Used as a base class to create specific packets.
	class OutPacket
	{
	public:
		// Construct a packet by writing its opcode.
		OutPacket(int16_t opcode);

		// Return the length in bytes.
		size_t length() const;
		// Return a const pointer to this packet's bytes.
		const int8_t* getbytes() const;

	protected:
		// Skip a number of bytes (filled with zeroes).
		void skip(size_t count);
		// Write a byte.
		void writech(int8_t ch);
		// Write a short.
		void writesh(int16_t sh);
		// Write an int.
		void writeint(int32_t in);
		// Write a long.
		void writelg(int64_t lg);
		// Write a string. Writes the length as a short
		// and then each individual character as a byte.
		void writestr(string str);
		// Write a point, one short for x and one for y.
		void writepoint(vector2d<int16_t> point);
		// Write the second count since epoch as an integer.
		void writetime();

	private:
		vector<int8_t> bytes;
	};
}

