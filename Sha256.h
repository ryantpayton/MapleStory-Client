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
	using::std::uint8_t;
	using::std::uint32_t;
	using::std::string;

	// Used to calculate hash values of game files. The server can then check with the client if all files are up-to-date.
	class Sha256
	{
	public:
		Sha256();
		~Sha256();
		// Calculates the hash value of data as byte array with the given length.
		// Parameters: uint8_t*(data in bytes), size_t(array length)
		string gethash(const uint8_t*, size_t) const;
		// Initialises a result array stored by the caller.
		void init(uint32_t*) const;
		// Calculates the hash value for data that is not fully read in. The size needs to congruent 0 modulo 64-byte.
		// Uses the array passed in to store the result.
		// Parameters: uint8_t*(data in bytes), size_t(array length), uint32_t*(result array)
		void feed(const uint8_t*, size_t, uint32_t*) const;
		// Processes the last bytes of data. Data is padded and does not need the correct size.
		// Parameters: uint8_t*(data in bytes), size_t(array length), uint32_t*(result array)
		void feedremains(const uint8_t*, size_t, uint32_t*) const;
		// Returns the result from the array passed in.
		string digest(uint32_t*) const;
	private:
		// Extends the byte array for processing in 64-byte chunks.
		void padbytes(uint8_t*, size_t) const;
		// Processes a 64-byte chunk of data to update hash value.
		void process(const uint8_t*, uint32_t*) const;
		// Rotate right.
		uint32_t rotr(uint32_t, size_t) const;
		// Rotate left.
		uint32_t rotl(uint32_t, size_t) const;
		// Shift right.
		uint32_t shfr(uint32_t, size_t) const;
	};
}