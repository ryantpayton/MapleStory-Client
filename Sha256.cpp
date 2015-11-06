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
#include "Sha256.h"
#include <math.h>
#include <iomanip>
#include <sstream>
#include <vector>

namespace Util
{
	const uint32_t fsqrp[8] =
	{
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};

	const uint32_t fcbrp[64] =
	{
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};

	Sha256::Sha256() {}

	Sha256::~Sha256() {}

	string Sha256::gethash(const uint8_t* bytes, size_t length) const
	{
		// Initialize result array.
		uint32_t hash[8];
		init(hash);
		// Feed all data.
		feedremains(bytes, length, hash);
		// Return result.
		return digest(hash);
	}

	void Sha256::init(uint32_t* hash) const
	{
		// Initialize result array.
		for (char i = 0; i < 8; i++)
		{
			hash[i] = fsqrp[i];
		}
	}

	void Sha256::feed(const uint8_t* bytes, size_t length, uint32_t* hash) const
	{
		// Process data in 64-byte chunks.
		size_t offset = 0;
		while (offset < length)
		{
			process(bytes + offset, hash);
			offset += 64;
		}
	}

	void Sha256::feedremains(const uint8_t* bytes, size_t length, uint32_t* hash) const
	{
		// Process data in 64-byte chunks.
		size_t offset = 0;
		size_t padsize = 64 - (length % 64);
		size_t total = length + padsize;
		while (offset < total)
		{
			// Process a 64-byte chunk
			if (total - offset > 64)
			{
				// The original array is still larger than the offset so no padding needed yet.
				process(bytes + offset, hash);
			}
			else
			{
				// We arrived at the last chunk so the array has to be padded to 64.
				uint8_t paddedbytes[64];
				size_t remain = length % 64;
				// Copy remaining data.
				memcpy(paddedbytes, bytes + offset, remain);
				// Pad it.
				padbytes(paddedbytes, remain);
				// Use the padded array as last chunk.
				process(paddedbytes, hash);
			}
			offset += 64;
		}
	}

	string Sha256::digest(uint32_t* hash) const
	{
		// Read hash values from the result array into a string, using hexadecimal format.
		using::std::ostringstream;
		ostringstream hashstr;
		hashstr << std::setfill('0') << std::setw(8);
		for (int i = 0; i < 8; i++)
		{
			hashstr << std::hex << hash[i];
		}
		return hashstr.str();
	}

	void Sha256::padbytes(uint8_t* buffer, size_t length) const
	{
		// First append the bit '1' to the array, then extend with '0' bits until the total length is 56.
		size_t padsize = 56 - length;
		buffer[length] = 128;
		for (size_t i = 1; i < padsize; i++)
		{
			buffer[length + i] = 0;
		}
		// Append the original length in bits (in big-endian) as a 64-bit integer.
		uint64_t blen64 = static_cast<uint64_t>(length * 8);
		for (size_t i = 7; i >= 0 && i < 8; i--)
		{
			buffer[length + padsize + i] = static_cast<uint8_t>(blen64);
			blen64 = blen64 >> 8;
		}
	}

	void Sha256::process(const uint8_t* bytes, uint32_t* hash) const
	{
		// Initialize a first array to work with. First insert the 64-byte chunk into the first 16 values.
		uint32_t words[64];
		for (int i = 0; i < 16; i++)
		{
			const uint8_t* cur = bytes + (i * 4);
			words[i] = (static_cast<uint32_t>(cur[0]) << 24) 
				| (static_cast<uint32_t>(cur[1]) << 16) 
				| (static_cast<uint32_t>(cur[2]) << 8) 
				| static_cast<uint32_t>(cur[3]);
		}
		// Initialize the rest of the array.
		for (int i = 16; i < 64; i++)
		{
			uint32_t va = words[i - 15];
			uint32_t vb = words[i - 2];
			uint32_t sa = rotr(va, 7) ^ rotr(va, 18) ^ (va >> 3);
			uint32_t sb = rotr(vb, 17) ^ rotr(vb, 19) ^ (vb >> 10);
			words[i] = words[i - 16] + sa + words[i - 7] + sb;
		}
		// Initialize working variables with the current hash result values.
		uint32_t work[8];
		for (int i = 0; i < 8; i++)
		{
			work[i] = hash[i];
		}
		// Hash algorithm.
		for (int i = 0; i < 64; i++)
		{
			uint32_t sa = rotr(work[0], 2) ^ rotr(work[0], 13) ^ rotr(work[0], 22);
			uint32_t sb = rotr(work[4], 6) ^ rotr(work[4], 11) ^ rotr(work[4], 25);
			uint32_t ch = (work[4] & work[5]) ^ ((~work[4]) & work[6]);
			uint32_t ma = (work[0] & work[1]) ^ (work[0] & work[2]) ^ (work[1] & work[2]);
			uint32_t ta = work[7] + sb + ch + fcbrp[i] + words[i];
			uint32_t tb = sa + ma;

			work[7] = work[6];
			work[6] = work[5];
			work[5] = work[4];
			work[4] = work[3] + ta;
			work[3] = work[2];
			work[2] = work[1];
			work[1] = work[0];
			work[0] = ta + tb;
		}
		// Finally, add values from working array to current result array.
		for (int i = 0; i < 8; i++)
		{
			hash[i] += work[i];
		}
	}

	uint32_t Sha256::rotr(uint32_t val, size_t count) const
	{
		return (val >> count) | (val << ((sizeof(val) << 3) - count));
	}

	uint32_t Sha256::rotl(uint32_t val, size_t count) const
	{
		return (val << count) | (val >> ((sizeof(val) << 3) - count));
	}
}