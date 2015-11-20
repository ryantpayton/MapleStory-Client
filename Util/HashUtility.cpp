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
#include "HashUtility.h"
#include "Sha256.h"
#include "xxhash.h"
#include <fstream>

namespace Util
{
	namespace HashUtility
	{
		// 128 MB.
		const size_t CHUNK_SIZE = 134217728;

		string getfilehash(const char* filename, uint64_t seed)
		{
			// Open file stream.
			using::std::ifstream;
			ifstream file(filename);

			uint64_t result;
			if (file.good())
			{
				// Get size of file.
				using::std::ios_base;
				file.seekg(0, ios_base::end);
				size_t end = file.tellg();
				file.seekg(0, ios_base::beg);

				if (end < CHUNK_SIZE)
				{
					// File size is smaller than chunk size so process all at once.
					char* buffer = new char[end];
					file.read(buffer, end);
					result = XXH64(buffer, end, seed);
					delete[] buffer;
				}
				else
				{
					// Read file in chunks instead.
					char* buffer = new char[CHUNK_SIZE];
					XXH64_state_t xxhstate;
					XXH_errorcode error;

					error = XXH64_reset(&xxhstate, seed);
					size_t offset = 0;
					while (offset < end && error == XXH_OK)
					{
						file.read(buffer, CHUNK_SIZE);
						error = XXH64_update(&xxhstate, buffer, CHUNK_SIZE);
						offset += CHUNK_SIZE;
					}
					size_t remaining = offset - end;
					if (remaining > 0)
					{
						file.read(buffer, CHUNK_SIZE);
						error = XXH64_update(&xxhstate, buffer, remaining);
					}

					if (error == XXH_OK)
					{
						result = XXH64_digest(&xxhstate);
					}
					else
					{
						result = 0;
					}
					delete[] buffer;
				}
			}
			else
			{
				result = 0;
			}

			file.close();
			return std::to_string(result);
		}

		string getfilehash(const char* filename)
		{
			// Open file stream.
			using::std::ifstream;
			ifstream file(filename);

			string result;
			if (file.good())
			{
				// Get size of file.
				using::std::ios_base;
				file.seekg(0, ios_base::end);
				size_t end = file.tellg();
				file.seekg(0, ios_base::beg);

				if (end < CHUNK_SIZE)
				{
					// File size is smaller than chunk size so process all at once.
					uint8_t* buffer = new uint8_t[end];
					file.read((char*)buffer, end);
					result = Sha256::gethash(buffer, end);
					delete[] buffer;
				}
				else
				{
					// Read file in chunks instead.
					uint8_t* buffer = new uint8_t[CHUNK_SIZE];
					uint32_t hash[8];
					Sha256::init(hash);

					size_t offset = 0;
					while (offset < end)
					{
						file.read((char*)buffer, CHUNK_SIZE);
						Sha256::feed(buffer, CHUNK_SIZE, hash);
						offset += CHUNK_SIZE;
					}
					size_t remaining = offset - end;
					if (remaining > 0)
					{
						file.read((char*)buffer, CHUNK_SIZE);
						Sha256::feedremains(buffer, CHUNK_SIZE, hash);
					}

					result = Sha256::digest(hash);
					delete[] buffer;
				}
			}
			else
			{
				result = "";
			}

			file.close();
			return result;
		}
	}
}
