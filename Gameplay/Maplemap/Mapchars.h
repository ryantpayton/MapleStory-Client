//////////////////////////////////////////////////////////////////////////////
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
#include "MapObjects.h"
#include "Character\OtherChar.h"
#include "Gameplay\Movement.h"
#include "Gameplay\Spawn.h"

namespace Gameplay
{
	using std::string;
	using std::vector;
	using Character::OtherChar;
	using Net::LookEntry;

	class MapChars : public MapObjects
	{
	public:
		void sendspawn(const CharSpawn& spawn);
		void removechar(int32_t cid);
		void movechar(int32_t cid, const vector<Movement>& movements);

		Optional<OtherChar> getchar(int32_t cid);
	};
}

