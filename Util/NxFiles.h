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

#include "../MapleStory.h"

#ifdef USE_NX
#include "../Error.h"

#include <array>

namespace ms
{
	namespace NxFiles
	{
		// Number of needed files
		constexpr uint8_t NUM_FILES = 29;

		// Names of the needed game files
		constexpr std::array<const char*, NUM_FILES> filenames =
		{
			"Base.nx",		// From latest
			"Character.nx",	// From latest
			"Effect.nx",	// From latest
			"Etc.nx",		// From latest
			"Item.nx",		// From latest
			"Map.nx",		// v83
			"MapLatest.nx", // From latest
			"Map001.nx",	// From latest
			"Map002.nx",	// From latest
			"Map2.nx",		// From latest
			"Mob.nx",		// From latest
			"Mob001.nx",	// From latest
			"Mob002.nx",	// From latest
			"Mob2.nx",		// From latest
			"Morph.nx",		// From latest
			"Npc.nx",		// From latest
			"Quest.nx",		// From latest
			"Reactor.nx",	// From latest
			"Skill.nx",		// From latest
			"Skill001.nx",	// From latest
			"Skill002.nx",	// From latest
			"Skill003.nx",	// From latest
			"Sound.nx",		// From latest
			"Sound001.nx",	// From latest
			"Sound002.nx",	// From latest
			"Sound2.nx",	// From latest
			"String.nx",	// From latest
			"TamingMob.nx",	// From latest
			"UI.nx"			// From latest
		};

		// Initialize NX
		// When successful test if the UI file is the correct version
		Error init();
	};
}
#endif