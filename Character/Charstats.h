//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Equipstat.h"
#include "Job.h"

#include "Inventory/Weapon.h"

#include "../Net/Login.h"
#include "../Template/EnumMap.h"
#include "../Template/Rectangle.h"

namespace ms
{
	class CharStats
	{
	public:
		CharStats(const StatsEntry& entry);
		CharStats();

		void init_totalstats();
		void set_stat(Maplestat::Id stat, std::uint16_t value);
		void set_total(Equipstat::Id stat, std::int32_t value);
		void add_buff(Equipstat::Id stat, std::int32_t value);
		void add_value(Equipstat::Id stat, std::int32_t value);
		void add_percent(Equipstat::Id stat, float percent);
		void close_totalstats();

		void set_weapontype(Weapon::Type weapontype);
		void set_exp(int64_t exp);
		void set_portal(std::uint8_t portal);
		void set_mastery(float mastery);
		void set_damagepercent(float damagepercent);
		void set_reducedamage(float reducedamage);

		void change_job(std::uint16_t id);

		std::int32_t calculate_damage(std::int32_t mobatk) const;

		bool is_damage_buffed() const;
		std::uint16_t get_stat(Maplestat::Id stat) const;
		std::int32_t get_total(Equipstat::Id stat) const;
		std::int32_t get_buffdelta(Equipstat::Id stat) const;
		Rectangle<int16_t> get_range() const;

		void set_mapid(std::int32_t id);
		std::int32_t get_mapid() const;
		std::uint8_t get_portal() const;
		int64_t get_exp() const;
		const std::string& get_name() const;
		const std::string& get_jobname() const;
		Weapon::Type get_weapontype() const;
		float get_mastery() const;
		float get_critical() const;
		float get_mincrit() const;
		float get_maxcrit() const;
		float get_reducedamage() const;
		float get_bossdmg() const;
		float get_ignoredef() const;
		void set_stance(float stance);
		float get_stance() const;
		float get_resistance() const;
		std::int32_t get_maxdamage() const;
		std::int32_t get_mindamage() const;
		std::uint16_t get_honor() const;
		void set_attackspeed(std::int8_t speed);
		std::int8_t get_attackspeed() const;
		const Job& get_job() const;
		bool get_female() const;

	private:
		std::int32_t calculateaccuracy() const;
		std::int32_t get_primary_stat() const;
		std::int32_t get_secondary_stat() const;
		float get_multiplier() const;

		std::string name;
		std::vector<int64_t> petids;
		Job job;
		int64_t exp;
		std::int32_t mapid;
		std::uint8_t portal;
		std::pair<std::int32_t, std::int8_t> rank;
		std::pair<std::int32_t, std::int8_t> jobrank;
		EnumMap<Maplestat::Id, std::uint16_t> basestats;
		EnumMap<Equipstat::Id, std::int32_t> totalstats;
		EnumMap<Equipstat::Id, std::int32_t> buffdeltas;
		EnumMap<Equipstat::Id, float> percentages;
		std::int32_t maxdamage;
		std::int32_t mindamage;
		std::uint16_t honor;
		std::int8_t attackspeed;
		std::int16_t projectilerange;
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
		bool female;
	};
}