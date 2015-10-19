/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
	StatsEntry::StatsEntry(InPacket& recv)
	{
		name = recv.readpadascii(13);
		recv.readbool(); //gender
		recv.readbyte(); //skin
		recv.readint(); //face
		recv.readint(); //hair
		for (uint8_t i = 0; i < 3; i++)
		{
			petids.push_back(recv.readlong());
		}
		stats[MS_LEVEL] = static_cast<uint8_t>(recv.readbyte());
		job = CharJob(recv.readshort());
		stats[MS_STR] = recv.readshort();
		stats[MS_DEX] = recv.readshort();
		stats[MS_INT] = recv.readshort();
		stats[MS_LUK] = recv.readshort();
		stats[MS_HP] = recv.readshort();
		stats[MS_MAXHP] = recv.readshort();
		stats[MS_MP] = recv.readshort();
		stats[MS_MAXMP] = recv.readshort();
		stats[MS_AP] = recv.readshort();
		stats[MS_MP] = recv.readshort();
		exp = recv.readint();
		stats[MS_FAME] = recv.readshort();
		recv.readint(); //gachaexp
		mapid = recv.readint();
		portal = recv.readbyte();
		recv.readint(); //timestamp
	}

	void StatsEntry::setrank(pair<int, char> r)
	{
		rank = r;
	}

	void StatsEntry::setjobrank(pair<int, char> jr)
	{
		jobrank = jr;
	}

	short StatsEntry::getstat(Maplestat stat) const
	{
		if (stat == MS_JOB)
		{
			return job.getid();
		}
		else
		{
			return (stats.count(stat) > 0) ? stats.at(stat) : 0;
		}
	}

	short StatsEntry::getjob() const
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

	map<Maplestat, short> StatsEntry::getstats() const
	{
		return stats;
	}

	int64_t StatsEntry::getexp() const
	{
		return exp;
	}

	int StatsEntry::getmapid() const
	{
		return mapid;
	}

	uint8_t StatsEntry::getportal() const
	{
		return portal;
	}

	pair<int, char> StatsEntry::getrank() const
	{
		return rank;
	}

	pair<int, char> StatsEntry::getjobrank() const
	{
		return jobrank;
	}
}
