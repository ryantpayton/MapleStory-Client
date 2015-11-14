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
#include "Net\Login\StatsEntry.h"
#include "Character\EquipConstants.h"
#include "Character\Look\Weapon.h"

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

		void calculatedamage(Weapon::WpType weapontype);
		void setstat(Maplestat type, uint16_t value);
		void settotal(Equipstat type, int32_t value);

		void setexp(int64_t ex) { exp = ex; }
		void setportal(uint8_t pt) { portal = pt; }
		void setattack(uint16_t at) { attack = at; }

		uint16_t calculateaccuracy() const;
		int64_t getexpneeded() const;
		uint16_t getstat(Maplestat) const;
		int32_t gettotal(Equipstat) const;

		int32_t getmapid() const { return mapid; }
		uint8_t getportal() const { return portal; }
		int64_t getexp() const { return exp; }
		string getname() const { return name; }
		string getjobname() const { return job.getname(); }
		float getmastery() const { return mastery; }
		float getcritical() const { return critical; }
		float getmincrit() const { return mincrit; }
		float getmaxcrit() const { return maxcrit; }
		float getbossdmg() const { return bossdmg; }
		float getignoredef() const { return ignoredef; }
		float getstance() const { return stance; }
		float getresist() const { return resiststatus; }
		int32_t getmaxdamage() const { return maxdamage; }
		int32_t getmindamage() const { return mindamage; }
		uint16_t getattack() const { return attack; }
		uint16_t gethonor() const { return honor; }

	private:
		uint16_t getprimary(Weapon::WpType weapontype);
		uint16_t getsecondary(Weapon::WpType weapontype);

		string name;
		vector<int64_t> petids;
		map<Maplestat, uint16_t> stats;
		CharJob job;
		int64_t exp;
		int32_t mapid;
		uint8_t portal;
		map<Equipstat, int32_t> totalstats;

		int32_t maxdamage;
		int32_t mindamage;
		uint16_t attack;
		uint16_t honor;
		float mastery;
		float critical;
		float mincrit;
		float maxcrit;
		float bossdmg;
		float ignoredef;
		float stance;
		float resiststatus;
	};
}

