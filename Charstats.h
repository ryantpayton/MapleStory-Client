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
#include "StatsEntry.h"
#include "EquipConstants.h"

namespace Character
{
	using::std::uint8_t;
	using::std::int16_t;
	using::std::int32_t;
	using::std::int64_t;
	using::std::vector;
	using::std::map;
	using::Net::StatsEntry;

	class Charstats
	{
	public:
		Charstats(const StatsEntry&);
		Charstats();
		~Charstats();
		void setstat(Maplestat, int16_t);
		void settotal(Equipstat, int32_t);
		void setexp(int64_t);
		void setportal(uint8_t);
		int32_t getmapid() const;
		uint8_t getportal() const;
		int16_t getstat(Maplestat) const;
		int32_t gettotal(Equipstat) const;
		string getname() const;
	private:
		string name;
		vector<int64_t> petids;
		map<Maplestat, int16_t> stats;
		CharJob job;
		int64_t exp;
		int32_t mapid;
		uint8_t portal;
		map<Equipstat, int32_t> totalstats;
	};
}

