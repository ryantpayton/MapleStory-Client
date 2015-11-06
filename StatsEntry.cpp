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
#include "StatsEntry.h"

namespace Net
{
	const size_t NUM_PETS = 3;

	StatsEntry::StatsEntry(InPacket& recv)
	{
		name = recv.readpadascii(13);

		recv.readbool(); //gender
		recv.readbyte(); //skin
		recv.readint(); //face
		recv.readint(); //hair

		for (size_t i = 0; i < NUM_PETS; i++)
		{
			petids.push_back(recv.readlong());
		}

		stats[Character::MS_LEVEL] = recv.readbyte();
		stats[Character::MS_JOB] = recv.readshort();
		stats[Character::MS_STR] = recv.readshort();
		stats[Character::MS_DEX] = recv.readshort();
		stats[Character::MS_INT] = recv.readshort();
		stats[Character::MS_LUK] = recv.readshort();
		stats[Character::MS_HP] = recv.readshort();
		stats[Character::MS_MAXHP] = recv.readshort();
		stats[Character::MS_MP] = recv.readshort();
		stats[Character::MS_MAXMP] = recv.readshort();
		stats[Character::MS_AP] = recv.readshort();
		stats[Character::MS_MP] = recv.readshort();
		exp = recv.readint();
		stats[Character::MS_FAME] = recv.readshort();

		recv.skip(4); //gachaexp
		mapid = recv.readint();
		portal = recv.readbyte();
		recv.skip(4); //timestamp

		job = CharJob(stats[Character::MS_JOB]);
	}

	StatsEntry::StatsEntry() {}

	StatsEntry::~StatsEntry() {}

	void StatsEntry::setrank(pair<int32_t, int8_t> r)
	{
		rank = r;
	}

	void StatsEntry::setjobrank(pair<int32_t, int8_t> jr)
	{
		jobrank = jr;
	}

	int16_t StatsEntry::getstat(Maplestat stat) const
	{
		return stats.count(stat) ? stats.at(stat) : 0;
	}

	int16_t StatsEntry::getjob() const
	{
		return job.getid();
	}

	string StatsEntry::getname() const
	{
		return name;
	}

	string StatsEntry::getjobname() const
	{
		return job.getname();
	}

	vector<int64_t> StatsEntry::getpets() const
	{
		return petids;
	}

	map<Maplestat, int16_t> StatsEntry::getstats() const
	{
		return stats;
	}

	int64_t StatsEntry::getexp() const
	{
		return exp;
	}

	int32_t StatsEntry::getmapid() const
	{
		return mapid;
	}

	uint8_t StatsEntry::getportal() const
	{
		return portal;
	}

	pair<int32_t, int8_t> StatsEntry::getrank() const
	{
		return rank;
	}

	pair<int32_t, int8_t> StatsEntry::getjobrank() const
	{
		return jobrank;
	}
}
