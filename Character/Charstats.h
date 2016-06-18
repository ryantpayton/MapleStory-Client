/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "Look\Weapon.h"

#include "..\Net\Login.h"
#include "..\Util\EnumMap.h"

namespace jrc
{
	class CharStats
	{
	public:
		CharStats(const StatsEntry& entry);
		CharStats();

		void init_totalstats();
		void set_stat(Maplestat::Value stat, uint16_t value);
		void set_total(Equipstat::Value stat, int32_t value);
		void add_buff(Equipstat::Value stat, int32_t value);
		void add_value(Equipstat::Value stat, int32_t value);
		void add_percent(Equipstat::Value stat, float percent);
		void close_totalstats();

		void set_weapontype(Weapon::Type weapontype);
		void set_exp(int64_t exp);
		void set_portal(uint8_t portal);
		void set_mastery(float mastery);
		void set_damagepercent(float damagepercent);
		void set_reducedamage(float reducedamage);

		void change_job(uint16_t id);

		bool isdamagebuffed() const;
		uint16_t get_stat(Maplestat::Value stat) const;
		int32_t get_total(Equipstat::Value stat) const;
		int32_t get_buffdelta(Equipstat::Value stat) const;
		Rectangle<int16_t> get_range() const;

		int32_t get_mapid() const;
		uint8_t get_portal() const;
		int64_t get_exp() const;
		const std::string& get_name() const;
		const std::string& get_jobname() const;
		Weapon::Type get_weapontype() const;
		float get_mastery() const;
		float get_critical() const;
		float get_mincrit() const { return mincrit; }
		float get_maxcrit() const { return maxcrit; }
		float get_bossdmg() const { return bossdmg; }
		float get_ignoredef() const { return ignoredef; }
		void set_stance(float st) { stance = st; }
		float get_stance() const { return stance; }
		float get_resistance() const { return resiststatus; }
		int32_t get_maxdamage() const { return maxdamage; }
		int32_t get_mindamage() const { return mindamage; }
		uint16_t gethonor() const { return honor; }
		void setattackspeed(int8_t as) { attackspeed = as; }
		int8_t get_attackspeed() const { return attackspeed; }
		const Job& get_job() const;

	private:
		int32_t calculateaccuracy() const;
		int32_t getprimary() const;
		int32_t getsecondary() const;
		float getmultiplier() const;

		std::string name;
		std::vector<int64_t> petids;
		Job job;
		int64_t exp;
		int32_t mapid;
		uint8_t portal;
		std::pair<int32_t, int8_t> rank;
		std::pair<int32_t, int8_t> jobrank;
		EnumMap<Maplestat::Value, uint16_t> basestats;
		EnumMap<Equipstat::Value, int32_t> totalstats;
		EnumMap<Equipstat::Value, int32_t> buffdeltas;
		EnumMap<Equipstat::Value, float> percentages;
		int32_t maxdamage;
		int32_t mindamage;
		uint16_t honor;
		int8_t attackspeed;
		int16_t projectilerange;
		Weapon::Type weapontype;
		float mastery;
		float critical;
		float mincrit;
		float maxcrit;
		float damagepercent;
		float bossdmg;
		float ignoredef;
		float stance;
		float resiststatus;
		float reducedamage;
	};
}

