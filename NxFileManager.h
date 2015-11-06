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

namespace Util
{
	const size_t NUM_FILES = 14;

	using::std::string;
	// Class for doing some work with game asset files.
	class NxFileManager
	{
	public:
		NxFileManager();
		~NxFileManager();
		// Makes sure that all game files exist. When successfull also tests if the UI file contains valid images.
		// Parameters: bool(calculate hash-values)
		bool init() const;
		// Obtains a hash value for a file of game assets. Fast version.
		// Parameters: size_t(index of file to hash), uint64_t(seed sent by the server)
		string gethash(size_t, uint64_t) const;
		// Obtains a hash value for a file of game assets. Slow version.
		// Parameters: size_t(index of file to hash)
		string gethash(size_t) const;
	private:
		// Checks if a game file exists.
		bool exists(size_t) const;
		HashUtility hashutil;
	};
}

