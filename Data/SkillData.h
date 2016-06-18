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
#pragma once
#include "Cache.h"

#include "..\Character\Look\Weapon.h"
#include "..\Graphics\Texture.h"

#include <array>
#include <unordered_map>

namespace jrc
{
	// Contains information about a skill.
	class SkillData : public Cache<SkillData>
	{
	public:
		// The stats of one level.
		struct Stats
		{
			float damage;
			int32_t matk;
			int32_t fixdamage;
			int32_t mastery;
			uint8_t attackcount;
			uint8_t mobcount;
			uint8_t bulletcount;
			int16_t bulletcost;
			int32_t hpcost;
			int32_t mpcost;
			float chance;
			float critical;
			float ignoredef;
			float hrange;
			Rectangle<int16_t> range;

			constexpr Stats(float damage, int32_t matk, int32_t fixdamage, int32_t mastery,
				uint8_t attackcount, uint8_t mobcount, uint8_t bulletcount, int16_t bulletcost,
				int32_t hpcost, int32_t mpcost, float chance, float critical, float ignoredef,
				float hrange, Rectangle<int16_t> range) :
				damage(damage), matk(matk), fixdamage(fixdamage), mastery(mastery),
				attackcount(attackcount), mobcount(mobcount), bulletcount(bulletcount), bulletcost(bulletcost),
				hpcost(hpcost), mpcost(mpcost), chance(chance), critical(critical), ignoredef(ignoredef),
				hrange(hrange), range(range) {}
		};

		// Icon types
		enum Type
		{
			NORMAL,
			DISABLED,
			MOUSEOVER,
			NUM_ICONS
		};

		// Return wether the skill is passive.
		bool is_passive() const;
		// Return wether the skill is an attack skill.
		bool is_offensive() const;
		// Return wether this skill is invisible in the skill book ui.
		bool is_invisible() const;
		// Return the default masterlevel.
		int32_t get_masterlevel() const;
		// Return the required weapon.
		Weapon::Type get_required_weapon() const;
		// Return the stats of one level.
		// If there are no stats for that level, a default object is returned.
		const Stats& get_stats(int32_t level) const;

		// Return the name of the skill.
		const std::string& get_name() const;
		// Return the description of the skill.
		const std::string& get_desc() const;
		// Return the description of a level.
		// If there is no description for this level, a warning message is returned.
		const std::string& get_level_desc(int32_t level) const;

		// Return one of the skill icons.
		// Cannot fail if type is a valid enum.
		const Texture& get_icon(Type type) const;

	private:
		friend Cache<SkillData>;
		SkillData(int32_t id);

		std::unordered_map<int32_t, Stats> stats;
		std::string element;
		Weapon::Type reqweapon;
		int32_t masterlevel;
		bool passive;
		bool offensive;
		bool invisible;

		std::string name;
		std::string desc;
		std::unordered_map<int32_t, std::string> levels;

		std::array<Texture, NUM_ICONS> icons;
	};
}