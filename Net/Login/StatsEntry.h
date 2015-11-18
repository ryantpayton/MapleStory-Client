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
#pragma once
#include "Net\InPacket.h"
#include "Character\CharConstants.h"
#include "Character\CharJob.h"
#include <map>
#include <vector>

namespace Net
{
	using std::vector;
	using std::map;
	using std::pair;
	using Character::Maplestat;
	using Character::CharJob;

	// Stats of a character directly constructed from a packet.
	class StatsEntry
	{
	public:
		StatsEntry(InPacket&);
		StatsEntry();
		~StatsEntry();

		void setrank(pair<int32_t, int8_t>);
		void setjobrank(pair<int32_t, int8_t>);
		int16_t getstat(Maplestat) const;
		int16_t getjob() const;
		string getname() const;
		string getjobname() const;
		vector<int64_t> getpets() const;
		map<Maplestat, uint16_t> getstats() const;
		int64_t getexp() const;
		int32_t getmapid() const;
		uint8_t getportal() const;
		pair<int32_t, int8_t> getrank() const;
		pair<int32_t, int8_t> getjobrank() const;

	private:
		string name;
		vector<int64_t> petids;
		map<Maplestat, uint16_t> stats;
		CharJob job;
		int64_t exp;
		int32_t mapid;
		uint8_t portal;
		pair<int32_t, int8_t> rank;
		pair<int32_t, int8_t> jobrank;
	};
}

