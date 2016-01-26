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
#include "NxFiles.h"
#include "HashUtility.h"
#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"
#include "nlnx\bitmap.hpp"
#include <string>
#include <fstream>

namespace Util
{
	// Names of game files in alphabetical order.
	const string filenames[NxFiles::NUM_FILES] =
	{
		"Character.nx", "Effect.nx", "Etc.nx", "Item.nx", "Map.nx", "Mob.nx", "Npc.nx",
		"Quest.nx", "Reactor.nx", "Skill.nx", "Sound.nx", "String.nx", "TamingMob.nx", "UI.nx"
	};

	bool NxFiles::exists(size_t index)
	{
		using std::ifstream;
		ifstream f;
		f.open(filenames[index].c_str());
		bool success = f.good();
		f.close();
		return success;
	}

	bool NxFiles::init()
	{
		for (size_t i = 0; i < NUM_FILES; i++)
		{
			if (!exists(i))
			{
				return false;
			}
		}

		nl::nx::load_all();

		using nl::bitmap;
		bitmap bmptest = nl::nx::ui["Login.img"]["WorldSelect"]["BtChannel"]["layer:bg"].get_bitmap();
		return bmptest.data() != nullptr;
	}

	string NxFiles::gethash(size_t index, uint64_t seed)
	{
		return (index < NUM_FILES) ? HashUtility::getfilehash(filenames[index].c_str(), seed) : 0;
	}

	string NxFiles::gethash(size_t index)
	{
		return (index < NUM_FILES) ? HashUtility::getfilehash(filenames[index].c_str()) : 0;
	}
}