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
#include "DamageEffect.h"

namespace jrc
{
	DamageEffect::DamageEffect(const SpecialMove& m, const AttackUser& u, 
		const DamageNumber& n, bool tl, int32_t dm, int32_t t, uint16_t d)
		: move(m), user(u), number(n), toleft(tl), damage(dm), target(t), delay(d) {}

	void DamageEffect::apply(Mob& mob) const
	{
		move.apply_hiteffects(user, mob);

		mob.apply_damage(damage, toleft);
	}

	bool DamageEffect::expired() const
	{
		return delay <= Constants::TIMESTEP;
	}

	bool DamageEffect::update()
	{
		if (delay <= Constants::TIMESTEP)
		{
			return true;
		}
		else
		{
			delay -= Constants::TIMESTEP;
			return false;
		}
	}

	int32_t DamageEffect::get_target() const
	{
		return target;
	}

	DamageNumber DamageEffect::get_number() const
	{
		return number;
	}
}