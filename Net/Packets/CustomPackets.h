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

#include "../OutPacket.h"

#ifdef USE_XXHASH
#include "../Util/HashUtility.h"
#include "../Util/NxFiles.h"
#endif

namespace ms
{
#ifdef USE_XXHASH
	// Packet which sends the hash values of all game files to the server.
	// Opcode: HASH_CHECK(30000)
	class NxCheckPacket : public OutPacket
	{
	public:
		NxCheckPacket(uint64_t seed) : OutPacket(HASH_CHECK)
		{
			write_byte(NxFiles::NUM_FILES);

			for (auto filename : NxFiles::filenames)
				write_string(HashUtility::get_filehash(filename, seed));
		}
	};
#endif
}