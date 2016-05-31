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
#include "PassiveBuffs.h"

#include "..\Gameplay\Combat\Skill.h"
#include "..\Util\Misc.h"

#include <nlnx\node.hpp>
#include <nlnx\nx.hpp>

namespace jrc
{
	bool ConditionlessBuff::is_applicable(CharStats&, nl::node) const
	{
		return true;
	}


	void AngelBlessingBuff::apply_to(CharStats& stats, nl::node level) const
	{
		stats.add_value(Equipstat::WATK, level["x"]);
		stats.add_value(Equipstat::MAGIC, level["y"]);
		stats.add_value(Equipstat::ACC, level["z"]);
		stats.add_value(Equipstat::AVOID, level["z"]);
	}


	template<Weapon::Type W1, Weapon::Type W2>
	bool f_is_applicable(CharStats& stats, nl::node level)
	{
		return f_is_applicable<W1>(stats, level)
			|| f_is_applicable<W2>(stats, level);
	}

	template<Weapon::Type W1>
	bool f_is_applicable(CharStats& stats, nl::node)
	{
		return stats.get_weapontype() == W1;
	}

	template <Weapon::Type...W>
	bool WeaponMasteryBuff<W...>::is_applicable(CharStats& stats, nl::node level) const
	{
		return f_is_applicable<W...>(stats, level);
	}

	template <Weapon::Type...W>
	void WeaponMasteryBuff<W...>::apply_to(CharStats& stats, nl::node level) const
	{
		float mastery = static_cast<float>(level["mastery"]) / 100;
		stats.set_mastery(mastery);
		stats.add_value(Equipstat::ACC, level["x"]);
	}


	void AchillesBuff::apply_to(CharStats& stats, nl::node level) const
	{
		float reducedamage = static_cast<float>(level["x"]) / 1000;
		stats.set_reducedamage(reducedamage);
	}


	bool BerserkBuff::is_applicable(CharStats& stats, nl::node level) const
	{
		float hp_percent = static_cast<float>(level["x"]) / 100;
		int32_t hp_threshold = static_cast<int32_t>(stats.gettotal(Equipstat::HP) * hp_percent);
		int32_t hp_current = stats.getstat(Maplestat::HP);
		return hp_current <= hp_threshold;
	}

	void BerserkBuff::apply_to(CharStats& stats, nl::node level) const
	{
		float damagepercent = static_cast<float>(level["damage"]) / 100;
		stats.set_damagepercent(damagepercent);
	}


	PassiveBuffs::PassiveBuffs()
	{
		// Beginner
		buffs[Skill::ANGEL_BLESSING] = std::make_unique<AngelBlessingBuff>();

		// Fighter
		buffs[Skill::SWORD_MASTERY_FIGHTER] = std::make_unique<WeaponMasteryBuff<Weapon::SWORD_1H, Weapon::SWORD_2H>>();
		buffs[Skill::AXE_MASTERY] = std::make_unique<WeaponMasteryBuff<Weapon::AXE_1H, Weapon::AXE_2H>>();

		// Crusader

		// Hero
		buffs[Skill::ACHILLES_HERO] = std::make_unique<AchillesBuff>();

		// Page
		buffs[Skill::SWORD_MASTERY_FIGHTER] = std::make_unique<WeaponMasteryBuff<Weapon::SWORD_1H, Weapon::SWORD_2H>>();
		buffs[Skill::BW_MASTERY] = std::make_unique<WeaponMasteryBuff<Weapon::MACE_1H, Weapon::MACE_2H>>();

		// White Knight

		// Paladin
		buffs[Skill::ACHILLES_PALADIN] = std::make_unique<AchillesBuff>();

		// Spearman
		buffs[Skill::SPEAR_MASTERY] = std::make_unique<WeaponMasteryBuff<Weapon::SPEAR>>();
		buffs[Skill::PA_MASTERY] = std::make_unique<WeaponMasteryBuff<Weapon::POLEARM>>();

		// Dragon Knight

		// Dark Knight
		buffs[Skill::ACHILLES_DK] = std::make_unique<AchillesBuff>();
		buffs[Skill::BERSERK] = std::make_unique<BerserkBuff>();
	}

	void PassiveBuffs::apply_buff(CharStats& stats, int32_t skill_id, int32_t skill_level) const
	{
		auto iter = buffs.find(skill_id);
		if (iter == buffs.end())
			return;

		bool wrong_job = !stats.get_job().can_use(skill_id);
		if (wrong_job)
			return;
		
		std::string strid;
		if (skill_id < 10000000)
		{
			strid = string_format::extend_id(skill_id, 7);
		}
		else
		{
			strid = std::to_string(skill_id);
		}
		nl::node src = nl::nx::skill[strid.substr(0, 3) + ".img"]["skill"][strid]["level"][skill_level];

		const PassiveBuff* buff = iter->second.get();
		if (buff && buff->is_applicable(stats, src))
		{
			buff->apply_to(stats, src);
		}
	}
}
