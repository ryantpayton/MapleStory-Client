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
#include "Timer.h"

namespace Character
{
	void Skillbook::setskill(int32_t id, int32_t level, int32_t mlevel, int64_t expire)
	{
		skillentries[id] = { level, mlevel, expire };
	}

	void Skillbook::setcd(int32_t id, int32_t cd)
	{
		cooldowns[id] = cd;
	}

	bool Skillbook::iscooling(int32_t id)
	{
		auto cditer = cooldowns.find(id);
		if (cditer != cooldowns.end())
		{
			if (cditer->second <= Timer::get().seconds())
			{
				cooldowns.erase(cditer);
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}

	bool Skillbook::hasskill(int32_t id) const
	{
		return skillentries.count(id) > 0;
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
