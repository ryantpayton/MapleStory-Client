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

namespace Util
{
	using::std::uint64_t;

	// Uses hashing to calculate the hash value of a game file. 
	class HashUtility
	{
	public:
		HashUtility();
		~HashUtility();
		// Calculate file hash using the fast xxhash algorithm. This requires a seed sent by the server.
		// Parameters: char*(name of file), uint64_t(seed)
		string getfilehash(const char*, uint64_t) const;
		// Calculate file hash using the slower Sha-256 algorithm.
		// Parameters: char*(name of file)
		string getfilehash(const char*) const;
	private:
		Sha256 sha256;
	};
}

