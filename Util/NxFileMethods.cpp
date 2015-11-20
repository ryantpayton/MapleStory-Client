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

#include "NxFileMethods.h"
#include "HashUtility.h"
#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"
#include "nlnx\bitmap.hpp"
#include <string>
#include <fstream>
#include <stdio.h>

namespace Util
{
	namespace NxFileMethods
	{
		// Names of game files in alphabetical order.
		const string nxfiles[NUM_FILES] =
		{
			"Character.nx", "Effect.nx", "Etc.nx", "Item.nx", "Map.nx", "Mob.nx", "Npc.nx",
			"Quest.nx", "Reactor.nx", "Skill.nx", "Sound.nx", "String.nx", "TamingMob.nx", "UI.nx"
		};

		bool exists(size_t index)
		{
			// Open file, store if loading successfull and close it.
			using::std::ifstream;
			ifstream f;
			f.open(nxfiles[index].c_str());
			bool success = f.good();
			f.close();
			return success;
		}

		bool init()
		{
			// Check if all files exists.
			for (size_t i = 0; i < NUM_FILES; i++)
			{
				if (!exists(i))
				{
					return false;
				}
			}

			// Initialise nolifenx. Load a test bitmap incase someone forgot 
			// to set the flag for including bitmaps in their .nx files.
			nl::nx::load_all();
			nl::bitmap bmptest = nl::nx::ui["Login.img"]["Common"]["frame"].get_bitmap();
			return bmptest.data() != nullptr;
		}

		string gethash(size_t index, uint64_t seed)
		{
			return (index < NUM_FILES) ? HashUtility::getfilehash(nxfiles[index].c_str(), seed) : 0;
		}

		string gethash(size_t index)
		{
			return (index < NUM_FILES) ? HashUtility::getfilehash(nxfiles[index].c_str()) : 0;
		}
	}
}