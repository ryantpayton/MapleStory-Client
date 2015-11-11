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
#include "Charstats.h"

namespace Character
{
	Charstats::Charstats(const StatsEntry& entry)
	{
		name = entry.getname();
		petids = entry.getpets();
		stats = entry.getstats();
		job = CharJob(entry.getjob());
		exp = entry.getexp();
		mapid = entry.getmapid();
		portal = entry.getportal();
	}

	Charstats::Charstats() {}

	Charstats::~Charstats() {}

	void Charstats::setstat(Maplestat stat, uint16_t value)
	{
		stats[stat] = value;
	}

	void Charstats::settotal(Equipstat stat, int32_t value)
	{
		totalstats[stat] = value;
	}

	void Charstats::setexp(int64_t value)
	{
		exp = value;
	}

	void Charstats::setportal(uint8_t pid)
	{
		portal = pid;
	}

	int32_t Charstats::getmapid() const
	{
		return mapid;
	}

	uint8_t Charstats::getportal() const
	{
		return portal;
	}

	uint16_t Charstats::getstat(Maplestat stat) const
	{
		return stats.count(stat) ? stats.at(stat) : 0;
	}

	int32_t Charstats::gettotal(Equipstat stat) const
	{
		return totalstats.count(stat) ? totalstats.at(stat) : 0;
	}

	string Charstats::getname() const
	{
		return name;
	}
}
