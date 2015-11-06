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
#include "Skillbook.h"

namespace Character
{
	Skillbook::Skillbook() {}

	Skillbook::~Skillbook() {}

	void Skillbook::setskill(int32_t id, int32_t level, int32_t mlevel, int64_t expire)
	{
		levels[id] = level;
		masterlevels[id] = mlevel;
		expirations[id] = expire;
	}

	void Skillbook::setcd(int32_t id, int32_t cd)
	{
		cooldowns[id] = cd;
	}
}
