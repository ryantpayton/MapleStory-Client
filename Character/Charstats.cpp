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
#include "CharStats.h"

#include "StatCaps.h"

namespace jrc
{
	CharStats::CharStats(const StatsEntry& s)
		: name(s.name), petids(s.petids),
		exp(s.exp), mapid(s.mapid), portal(s.portal),
		rank(s.rank), jobrank(s.jobrank), basestats(s.stats) {

		job = basestats[Maplestat::JOB];
		init_totalstats();
	}

	CharStats::CharStats() {}

	void CharStats::init_totalstats()
	{
		totalstats.clear();
		buffdeltas.clear();
		percentages.clear();

		totalstats[Equipstat::HP] = get_stat(Maplestat::MAXHP);
		totalstats[Equipstat::MP] = get_stat(Maplestat::MAXMP);
		totalstats[Equipstat::STR] = get_stat(Maplestat::STR);
		totalstats[Equipstat::DEX] = get_stat(Maplestat::DEX);
		totalstats[Equipstat::INT] = get_stat(Maplestat::INT);
		totalstats[Equipstat::LUK] = get_stat(Maplestat::LUK);
		totalstats[Equipstat::SPEED] = 100;
		totalstats[Equipstat::JUMP] = 100;

		maxdamage = 0;
		mindamage = 0;
		honor = 0;
		attackspeed = 0;
		projectilerange = 400;
		mastery = 0.0f;
		critical = 0.05f;
		mincrit = 0.5f;
		maxcrit = 0.75f;
		damagepercent = 0.0f;
		bossdmg = 0.0f;
		ignoredef = 0.0f;
		stance = 0.0f;
		resiststatus = 0.0f;
		reducedamage = 0.0f;
	}

	void CharStats::close_totalstats()
	{
		totalstats[Equipstat::ACC] += calculateaccuracy();

		for (auto iter : percentages)
		{
			Equipstat::Id stat = iter.first;
			int32_t total = totalstats[stat];
			total += static_cast<int32_t>(total * iter.second);
			set_total(stat, total);
		}

		int32_t primary = get_primary_stat();
		int32_t secondary = get_secondary_stat();
		int32_t attack = get_total(Equipstat::WATK);
		float multiplier = damagepercent + static_cast<float>(attack) / 100;
		maxdamage = static_cast<int32_t>((primary + secondary) * multiplier);
		mindamage = static_cast<int32_t>(((primary * 0.9f * mastery) + secondary) * multiplier);
	}

	int32_t CharStats::calculateaccuracy() const
	{
		int32_t totaldex = get_total(Equipstat::DEX);
		int32_t totalluk = get_total(Equipstat::LUK);
		return static_cast<int32_t>(totaldex * 0.8f + totalluk * 0.5f);
	}

	int32_t CharStats::get_primary_stat() const
	{
		Equipstat::Id primary = job.get_primary(weapontype);
		return static_cast<int32_t>(get_multiplier() * get_total(primary));
	}

	int32_t CharStats::get_secondary_stat() const
	{
		Equipstat::Id secondary = job.get_secondary(weapontype);
		return get_total(secondary);
	}

	float CharStats::get_multiplier() const
	{
		switch (weapontype)
		{
		case Weapon::SWORD_1H:
			return 4.0f;
		case Weapon::AXE_1H:
		case Weapon::MACE_1H:
		case Weapon::WAND:
		case Weapon::STAFF:
			return 4.4f;
		case Weapon::DAGGER:
		case Weapon::CROSSBOW:
		case Weapon::CLAW:
		case Weapon::GUN:
			return 3.6f;
		case Weapon::SWORD_2H:
			return 4.6f;
		case Weapon::AXE_2H:
		case Weapon::MACE_2H:
		case Weapon::KNUCKLE:
			return 4.8f;
		case Weapon::SPEAR:
		case Weapon::POLEARM:
			return 5.0f;
		case Weapon::BOW:
			return 3.4f;
		default:
			return 0.0f;
		}
	}

	void CharStats::set_stat(Maplestat::Id stat, uint16_t value)
	{
		basestats[stat] = value;
	}

	void CharStats::set_total(Equipstat::Id stat, int32_t value)
	{
		auto iter = EQSTAT_CAPS.find(stat);
		if (iter != EQSTAT_CAPS.end())
		{
			int32_t cap_value = iter->second;

			if (value > cap_value)
				value = cap_value;
		}

		totalstats[stat] = value;
	}

	void CharStats::add_buff(Equipstat::Id stat, int32_t value)
	{
		int32_t current = get_total(stat);
		set_total(stat, current + value);
		buffdeltas[stat] += value;
	}

	void CharStats::add_value(Equipstat::Id stat, int32_t value)
	{
		int32_t current = get_total(stat);
		set_total(stat, current + value);
	}

	void CharStats::add_percent(Equipstat::Id stat, float percent)
	{
		percentages[stat] += percent;
	}

	void CharStats::set_weapontype(Weapon::Type w)
	{
		weapontype = w;
	}

	void CharStats::set_exp(int64_t e)
	{
		exp = e;
	}

	void CharStats::set_portal(uint8_t p)
	{
		portal = p;
	}

	void CharStats::set_mastery(float m)
	{
		mastery = 0.5f + m;
	}

	void CharStats::set_damagepercent(float d)
	{
		damagepercent = d;
	}

	void CharStats::set_reducedamage(float r)
	{
		reducedamage = r;
	}

	void CharStats::change_job(uint16_t id)
	{
		basestats[Maplestat::JOB] = id;
		job.change_job(id);
	}

	int32_t CharStats::calculate_damage(int32_t mobatk) const
	{
		// random stuff, need to find the actual formula somewhere
		int32_t reduceatk = mobatk / 2 + mobatk / get_total(Equipstat::WDEF);
		return reduceatk - static_cast<int32_t>(reduceatk * reducedamage);
	}

	bool CharStats::is_damage_buffed() const
	{
		return get_buffdelta(Equipstat::WATK) > 0
			|| get_buffdelta(Equipstat::MAGIC) > 0;
	}

	uint16_t CharStats::get_stat(Maplestat::Id stat) const
	{
		return basestats[stat];
	}

	int32_t CharStats::get_total(Equipstat::Id stat) const
	{
		return totalstats[stat];
	}

	int32_t CharStats::get_buffdelta(Equipstat::Id stat) const
	{
		return buffdeltas[stat];
	}

	Rectangle<int16_t> CharStats::get_range() const
	{
		return Rectangle<int16_t>(-projectilerange, -5, -50, 50);
	}

	int32_t CharStats::get_mapid() const
	{
		return mapid;
	}

	uint8_t CharStats::get_portal() const
	{
		return portal;
	}

	int64_t CharStats::get_exp() const
	{
		return exp;
	}

	const std::string& CharStats::get_name() const
	{
		return name;
	}

	const std::string& CharStats::get_jobname() const
	{
		return job.get_name();
	}

	Weapon::Type CharStats::get_weapontype() const
	{
		return weapontype;
	}

	float CharStats::get_mastery() const
	{
		return mastery;
	}

	float CharStats::get_critical() const
	{
		return critical;
	}

	float CharStats::get_mincrit() const
	{
		return mincrit;
	}

	float CharStats::get_maxcrit() const
	{
		return maxcrit;
	}

	float CharStats::get_reducedamage() const
	{
		return reducedamage;
	}

	float CharStats::get_bossdmg() const
	{
		return bossdmg;
	}

	float CharStats::get_ignoredef() const
	{
		return ignoredef;
	}

	void CharStats::set_stance(float s)
	{
		stance = s;
	}

	float CharStats::get_stance() const
	{
		return stance;
	}

	float CharStats::get_resistance() const
	{
		return resiststatus;
	}

	int32_t CharStats::get_maxdamage() const
	{
		return maxdamage;
	}

	int32_t CharStats::get_mindamage() const
	{
		return mindamage;
	}

	uint16_t CharStats::get_honor() const
	{
		return honor;
	}

	void CharStats::set_attackspeed(int8_t as)
	{
		attackspeed = as;
	}

	int8_t CharStats::get_attackspeed() const
	{
		return attackspeed;
	}

	const Job& CharStats::get_job() const
	{
		return job;
	}
}
