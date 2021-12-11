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
		constexpr uint8_t NUM_FILES = 28;

		// Names of the needed game files
		constexpr std::array<const char*, NUM_FILES> filenames =
		{
			"Base.nx",
			"Character.nx",
			"Effect.nx",
			"Etc.nx",
			"Item.nx",
			"Map.nx",
			"Map001.nx",
			"Map002.nx",
			"Map2.nx",
			"Mob.nx",
			"Mob001.nx",
			"Mob002.nx",
			"Mob2.nx",
			"Morph.nx",
			"Npc.nx",
			"Quest.nx",
			"Reactor.nx",
			"Skill.nx",
			"Skill001.nx",
			"Skill002.nx",
			"Skill003.nx",
			"Sound.nx",
			"Sound001.nx",
			"Sound002.nx",
			"Sound2.nx",
			"String.nx",
			"TamingMob.nx",
			"UI.nx"
		};

		// Initialize NX
		// When successful test if the UI file is the correct version
		Error init();
	};
}
#endif