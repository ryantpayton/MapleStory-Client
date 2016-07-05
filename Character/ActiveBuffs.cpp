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
	template <Equipstat::Id STAT>
	void SimpleStatBuff<STAT>::apply_to(CharStats& stats, int16_t value) const
	{
		stats.add_buff(STAT, value);
	}


	template <Equipstat::Id STAT>
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
		stats.set_attackspeed(static_cast<int8_t>(value));
	}


	ActiveBuffs::ActiveBuffs()
	{
		buffs[Buffstat::MAPLE_WARRIOR] = std::make_unique<MapleWarriorBuff>();
		buffs[Buffstat::STANCE] = std::make_unique<StanceBuff>();
		buffs[Buffstat::BOOSTER] = std::make_unique<BoosterBuff>();
		buffs[Buffstat::WATK] = std::make_unique<SimpleStatBuff<Equipstat::WATK>>();
		buffs[Buffstat::WDEF] = std::make_unique<SimpleStatBuff<Equipstat::WDEF>>();
		buffs[Buffstat::MATK] = std::make_unique<SimpleStatBuff<Equipstat::MAGIC>>();
		buffs[Buffstat::MDEF] = std::make_unique<SimpleStatBuff<Equipstat::MDEF>>();
		buffs[Buffstat::SPEED] = std::make_unique<SimpleStatBuff<Equipstat::SPEED>>();
		buffs[Buffstat::JUMP] = std::make_unique<SimpleStatBuff<Equipstat::JUMP>>();
		buffs[Buffstat::HYPERBODYHP] = std::make_unique<PercentageStatBuff<Equipstat::HP>>();
		buffs[Buffstat::HYPERBODYMP] = std::make_unique<PercentageStatBuff<Equipstat::MP>>();
	}

	void ActiveBuffs::apply_buff(CharStats& stats, Buffstat::Id stat, int16_t value) const
	{
		if (auto& buff = buffs[stat])
		{
			buff->apply_to(stats, value);
		}
	}
}