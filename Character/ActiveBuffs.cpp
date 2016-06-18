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
#include "ActiveBuffs.h"

namespace jrc
{
	template <Equipstat::Value STAT>
	void SimpleStatBuff<STAT>::apply_to(CharStats& stats, int16_t value) const
	{
		stats.add_buff(STAT, value);
	}


	template <Equipstat::Value STAT>
	void PercentageStatBuff<STAT>::apply_to(CharStats& stats, int16_t value) const
	{
		stats.add_percent(STAT, static_cast<float>(value) / 100);
	}


	void MapleWarriorBuff::apply_to(CharStats& stats, int16_t value) const
	{
		stats.add_percent(Equipstat::STR, static_cast<float>(value) / 100);
		stats.add_percent(Equipstat::DEX, static_cast<float>(value) / 100);
		stats.add_percent(Equipstat::INT, static_cast<float>(value) / 100);
		stats.add_percent(Equipstat::LUK, static_cast<float>(value) / 100);
	}


	void StanceBuff::apply_to(CharStats& stats, int16_t value) const
	{
		stats.set_stance(static_cast<float>(value) / 100);
	}


	void BoosterBuff::apply_to(CharStats& stats, int16_t value) const
	{
		stats.setattackspeed(static_cast<int8_t>(value));
	}


	ActiveBuffs::ActiveBuffs()
	{
		buffs[Buff::MAPLE_WARRIOR] = std::make_unique<MapleWarriorBuff>();
		buffs[Buff::STANCE] = std::make_unique<StanceBuff>();
		buffs[Buff::BOOSTER] = std::make_unique<BoosterBuff>();
		buffs[Buff::WATK] = std::make_unique<SimpleStatBuff<Equipstat::WATK>>();
		buffs[Buff::WDEF] = std::make_unique<SimpleStatBuff<Equipstat::WDEF>>();
		buffs[Buff::MATK] = std::make_unique<SimpleStatBuff<Equipstat::MAGIC>>();
		buffs[Buff::MDEF] = std::make_unique<SimpleStatBuff<Equipstat::MDEF>>();
		buffs[Buff::SPEED] = std::make_unique<SimpleStatBuff<Equipstat::SPEED>>();
		buffs[Buff::JUMP] = std::make_unique<SimpleStatBuff<Equipstat::JUMP>>();
		buffs[Buff::HYPERBODYHP] = std::make_unique<PercentageStatBuff<Equipstat::HP>>();
		buffs[Buff::HYPERBODYMP] = std::make_unique<PercentageStatBuff<Equipstat::MP>>();
	}

	void ActiveBuffs::apply_buff(CharStats& stats, Buff::Stat stat, int16_t value) const
	{
		auto iter = buffs.find(stat);
		if (iter == buffs.end())
			return;

		const ActiveBuff* buff = iter->second.get();
		if (buff)
		{
			buff->apply_to(stats, value);
		}
	}
}