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
#include <string>
#include <cstdint>

namespace Util
{
	// Functions for Sha256 encryption.
	namespace Sha256
	{
		using std::uint8_t;
		using std::uint32_t;
		using std::string;

		// Calculates the hash value of data as byte array with the given length.
		// Parameters: uint8_t*(data in bytes), size_t(array length)
		string gethash(const uint8_t*, size_t);

		// Initialises a result array stored by the caller.
		void init(uint32_t*);
		// Calculates the hash value for data that is not fully read in. The size needs to congruent 0 modulo 64-byte.
		// Uses the array passed in to store the result.
		// Parameters: uint8_t*(data in bytes), size_t(array length), uint32_t*(result array)
		void feed(const uint8_t*, size_t, uint32_t*);
		// Processes the last bytes of data. Data is padded and does not need the correct size.
		// Parameters: uint8_t*(data in bytes), size_t(array length), uint32_t*(result array)
		void feedremains(const uint8_t*, size_t, uint32_t*);
		// Returns the result from the array passed in.
		string digest(uint32_t*);
	}
}