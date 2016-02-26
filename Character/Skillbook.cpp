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
	void Skillbook::setskill(int32_t id, int32_t level, int32_t mlevel, int64_t expire)
	{
		SkillEntry entry;
		entry.level = level;
		entry.masterlevel = mlevel;
		entry.expiration = expire;
		entry.cooldown = 0;
		skillentries[id] = entry;
	}

	bool Skillbook::hasskill(int32_t id) const
	{
		return skillentries.count(id) > 0;
	}

	void Skillbook::setcd(int32_t id, int32_t cd)
	{
		if (hasskill(id))
		{
			skillentries[id].cooldown = cd;
		}
	}

	bool Skillbook::iscooling(int32_t id) const
	{
		return hasskill(id) ? (skillentries.at(id).cooldown > 0) : true;
	}

	int32_t Skillbook::getlevel(int32_t id) const
	{
		return hasskill(id) ? skillentries.at(id).level : 0;
	}

	int32_t Skillbook::getmasterlevel(int32_t id) const
	{
		return hasskill(id) ? skillentries.at(id).masterlevel : 0;
	}
}
