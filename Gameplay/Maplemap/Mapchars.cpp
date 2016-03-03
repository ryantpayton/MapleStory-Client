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
#include "MapChars.h"

namespace Gameplay
{
	MapChars::MapChars() {}

	void MapChars::addchar(int32_t cid, const LookEntry& look, uint8_t level,
		int16_t job, string name, int8_t stance, Point<int16_t> pos) {

		add(new OtherChar(cid, look, level, job, name, stance, pos));
	}

	void MapChars::removechar(int32_t cid)
	{
		remove(cid);
	}

	void MapChars::movechar(int32_t cid, const vector<Movement>& movements)
	{
		Optional<OtherChar> otherchar = getchar(cid);
		if (otherchar)
		{
			otherchar->sendmovement(movements);
		}
	}

	Optional<OtherChar> MapChars::getchar(int32_t cid)
	{
		return get(cid)
			.reinterpret<OtherChar>();
	}
}
