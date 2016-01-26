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
#include "Util\Singleton.h"
#include <cstdint>
#include <string>

namespace Util
{
	using std::string;

	class NxFiles : public Singleton<NxFiles>
	{
	public:
		static const size_t NUM_FILES = 14;

		// Makes sure that all game files exist. 
		// When successfull also tests if the UI file contains valid images.
		bool init();

		// Obtains a hash value for a file of game assets. Fast version.
		string gethash(size_t fileindex, uint64_t seed);
		// Obtains a hash value for a file of game assets. Slow version.
		string gethash(size_t fileindex);

	private:
		bool exists(size_t index);
	};
}

