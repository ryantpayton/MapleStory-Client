/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "NxFiles.h"
#include "HashUtility.h"

#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"
#include "nlnx\bitmap.hpp"

#include <string>
#include <fstream>

namespace jrc
{
	bool NxFiles::exists(const char* filename)
	{
		std::ifstream f(filename);
		return f.good();
	}

	Error NxFiles::init()
	{
		for (auto filename : filenames)
		{
			if (!exists(filename))
			{
				return Error::MISSING_FILE;
			}
		}

		nl::nx::load_all();

		nl::bitmap bmptest = nl::nx::ui["Login.img"]["WorldSelect"]["BtChannel"]["layer:bg"];
		if (!bmptest.data())
			return Error::WRONG_UI_FILE;

		return Error::NONE;
	}

#ifdef JOURNEY_USE_XXHASH
	std::queue<std::string> NxFiles::gethashes(uint64_t seed)
	{
		std::queue<std::string> hashes;
		for (auto& filename : filenames.values())
		{
			hashes.push(
				HashUtility::getfilehash(filename, seed)
				);
		}
		return hashes;
	}
#endif
}