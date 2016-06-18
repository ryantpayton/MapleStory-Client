//////////////////////////////////////////////////////////////////////////////
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
#include "SkillData.h"

#include "..\Util\Misc.h"

#include <nlnx\nx.hpp>
#include <nlnx\node.hpp>

namespace jrc
{
	SkillData::SkillData(int32_t id)
	{
		// Locate sources
		std::string strid = string_format::extend_id(id, 7);
		nl::node src = nl::nx::skill[strid.substr(0, 3) + ".img"]["skill"][strid];
		nl::node strsrc = nl::nx::string["Skill.img"][strid];


		// Load icons
		icons = { src["icon"], src["iconDisabled"], src["iconMouseOver"] };


		// Load strings
		name = strsrc["name"];
		desc = strsrc["desc"];

		for (int32_t level = 1; nl::node sub = strsrc["h" + std::to_string(level)]; level++)
		{
			levels.emplace(level, sub);
		}


		// Load stats
		nl::node levelsrc = src["level"];
		for (auto sub : levelsrc)
		{
			float damage = (float)sub["damage"] / 100;
			int32_t matk = sub["mad"];
			int32_t fixdamage = sub["fixdamage"];
			int32_t mastery = sub["mastery"];
			uint8_t  attackcount = (uint8_t)sub["attackCount"].get_integer(1);
			uint8_t  mobcount = (uint8_t)sub["mobCount"].get_integer(1);
			uint8_t bulletcount = (uint8_t)sub["bulletCount"].get_integer(1);
			int16_t bulletcost = (int16_t)sub["bulletConsume"].get_integer(bulletcount);
			int32_t hpcost = sub["hpCon"];
			int32_t mpcost = sub["mpCon"];
			float chance = (float)sub["prop"].get_real(100.0) / 100;
			float critical = 0.0f;
			float ignoredef = 0.0f;
			float hrange = (float)sub["range"].get_real(100.0) / 100;
			Rectangle<int16_t> range = sub;
			int32_t level = string_conversion::or_default<int32_t>(sub.name(), -1);
			stats.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(level),
				std::forward_as_tuple(damage, matk, fixdamage, mastery, attackcount, mobcount,
					bulletcount, bulletcost, hpcost, mpcost, chance, critical, ignoredef, hrange, range)
			);
		}

		element = src["elemAttr"];
		reqweapon = Weapon::typebyvalue(100 + (int32_t)src["weapon"]);
		masterlevel = static_cast<int32_t>(stats.size());
		passive = (id % 10000) / 1000 == 0;
		bool hasdamage = levelsrc["1"]["damage"].data_type() == nl::node::type::integer;
		bool hasmad = levelsrc["1"]["mad"].data_type() == nl::node::type::integer;
		bool hasfixdamage = levelsrc["1"]["fixdamage"].data_type() == nl::node::type::integer;
		offensive = !passive && (hasdamage || hasmad || hasfixdamage);
		invisible = src["invisible"].get_bool();
	}

	bool SkillData::is_passive() const
	{
		return passive;
	}

	bool SkillData::is_offensive() const
	{
		return offensive;
	}

	bool SkillData::is_invisible() const
	{
		return invisible;
	}

	int32_t SkillData::get_masterlevel() const
	{
		return masterlevel;
	}

	Weapon::Type SkillData::get_required_weapon() const
	{
		return reqweapon;
	}

	const SkillData::Stats& SkillData::get_stats(int32_t level) const
	{
		auto iter = stats.find(level);
		if (iter == stats.end())
		{
			static constexpr Stats null_stats(0.0f, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, {});
			return null_stats;
		}
		return iter->second;
	}

	const std::string& SkillData::get_name() const
	{
		return name;
	}

	const std::string& SkillData::get_desc() const
	{
		return desc;
	}

	const std::string& SkillData::get_level_desc(int32_t level) const
	{
		auto iter = levels.find(level);
		if (iter == levels.end())
		{
			static const std::string null_level = "Missing level description.";
			return null_level;
		}
		else
		{
			return iter->second;
		}
	}

	const Texture& SkillData::get_icon(Type type) const
	{
		return icons[type];
	}


	std::unordered_map<int32_t, SkillData> SkillData::cache;
}