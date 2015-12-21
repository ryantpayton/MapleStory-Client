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
#include "Gameplay\MovementInfo.h"

namespace Gameplay
{
	using std::string;
	using std::vector;
	using Character::OtherChar;
	using Net::LookEntry;

	class MapChars : public MapObjects
	{
	public:
		MapChars();

		void addchar(int32_t cid, const LookEntry& look, uint8_t level, 
			int16_t job, string name, int8_t stance, vector2d<int16_t> pos);
		void removechar(int32_t cid);
		void movechar(int32_t cid, const vector<MovementFragment>& movements);

		OtherChar* getchar(int32_t cid);
	};
}

