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
#pragma once
#include "InPacket.h"
#include "CharConstants.h"
#include "CharJob.h"
#include <map>

using namespace Constants;
using namespace Character;

namespace Net
{
	class StatsEntry
	{
	public:
		StatsEntry(InPacket&);
		StatsEntry() {}
		~StatsEntry() {}
		void setrank(pair<int, char>);
		void setjobrank(pair<int, char>);
		short getstat(Maplestat) const;
		short getjob() const;
		string getname() const;
		string getjobname() const;
		vector<int64_t> getpets() const;
		map<Maplestat, short> getstats() const;
		int64_t getexp() const;
		int getmapid() const;
		uint8_t getportal() const;
		pair<int, char> getrank() const;
		pair<int, char> getjobrank() const;
	private:
		string name;
		vector<int64_t> petids;
		map<Maplestat, short> stats;
		CharJob job;
		int64_t exp;
		int mapid;
		uint8_t portal;
		pair<int, char> rank;
		pair<int, char> jobrank;
	};
}

