//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "HashUtility.h"

#ifdef USE_XXHASH
#include <xxhash.h>
#include <fstream>

namespace ms
{
	namespace HashUtility
	{
		// 128 MB.
		const size_t CHUNK_SIZE = 134217728;

		std::string get_filehash(const char* filename, uint64_t seed)
		{
			// Open file stream.
			std::ifstream file(filename);

			uint64_t result;

			if (file.good())
			{
				// Get size of file.
				file.seekg(0, std::ios_base::end);
				size_t end = file.tellg();
				file.seekg(0, std::ios_base::beg);

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
						result = XXH64_digest(&xxhstate);
					else
						result = 0;

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
	}
}
#endif