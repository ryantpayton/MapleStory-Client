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
#include "Net\Login.h"
#include "Character\Look\Weapon.h"

namespace Character
{
	using std::vector;
	using std::map;
	using Net::StatsEntry;

	class CharStats
	{
	public:
		CharStats(StatsEntry);
		CharStats();
		~CharStats();

		void inittotalstats();
		void setstat(Maplestat::Value stat, uint16_t value);
		void settotal(Equipstat::Value stat, int32_t value);
		void addtotal(Equipstat::Value stat, int32_t value);
		void calculatedamage(Weapon::Type weapontype);

		uint16_t calculateaccuracy() const;
		int64_t getexpneeded() const;
		uint16_t getstat(Maplestat::Value stat) const;
		int32_t gettotal(Equipstat::Value stat) const;

		void setexp(int64_t ex) { stats.exp = ex; }
		void setportal(uint8_t pt) { stats.portal = pt; }

		int32_t getmapid() const { return stats.mapid; }
		uint8_t getportal() const { return stats.portal; }
		int64_t getexp() const { return stats.exp; }
		string getname() const { return stats.name; }
		string getjobname() const { return stats.job.getname(); }
		float getmastery() const { return mastery; }
		float getcritical() const { return critical; }
		float getmincrit() const { return mincrit; }
		float getmaxcrit() const { return maxcrit; }
		float getbossdmg() const { return bossdmg; }
		float getignoredef() const { return ignoredef; }
		void setstance(float st) { stance = st; }
		float getstance() const { return stance; }
		float getresist() const { return resiststatus; }
		int32_t getmaxdamage() const { return maxdamage; }
		int32_t getmindamage() const { return mindamage; }
		uint16_t gethonor() const { return honor; }
		void setattackspeed(int8_t as) { attackspeed = as; }
		int8_t getattackspeed() const { return attackspeed; }

	private:
		int32_t getprimary(Weapon::Type weapontype) const;
		int32_t getsecondary(Weapon::Type weapontype) const;
		float getmultiplier(Weapon::Type weapontype) const;

		StatsEntry stats;

		map<Equipstat::Value, int32_t> totalstats;
		int32_t maxdamage;
		int32_t mindamage;
		uint16_t honor;
		int8_t attackspeed;
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

