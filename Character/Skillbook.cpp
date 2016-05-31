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

#include "..\Timer.h"

#include <algorithm>

namespace jrc
{
	void Skillbook::set_skill(int32_t id, int32_t level, int32_t mlevel, int64_t expire)
	{
		skillentries[id] = { level, mlevel, expire };
	}

	void Skillbook::set_cd(int32_t id, int32_t cd)
	{
		cooldowns[id] = cd;
	}

	bool Skillbook::is_cooling(int32_t id)
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

	bool Skillbook::has_skill(int32_t id) const
	{
		return skillentries.count(id) > 0;
	}

	int32_t Skillbook::get_level(int32_t id) const
	{
		return has_skill(id) ? skillentries.at(id).level : 0;
	}

	int32_t Skillbook::get_masterlevel(int32_t id) const
	{
		return has_skill(id) ? skillentries.at(id).masterlevel : 0;
	}

	std::map<int32_t, int32_t> Skillbook::collect_passives() const
	{
		std::map<int32_t, int32_t> passives;
		for (auto& iter : skillentries)
		{
			bool passive = is_passive(iter.first);
			if (passive)
			{
				passives.emplace(iter.first, iter.second.level);
			}
		}
		return passives;
	}
}
