//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2016 Daniel Allendorf                                        //
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
#include "SkillHitEffect.h"

#include "Util\Misc.h"

namespace Gameplay
{
	SingleHitEffect::SingleHitEffect(node src)
	{
		effect = src["hit"]["0"];
	}

	void SingleHitEffect::apply(Mob& target, uint16_t, bool) const
	{
		target.showeffect(effect.animation, effect.pos);
	}


	TwoHHitEffect::TwoHHitEffect(node src)
	{
		effects[false] = src["hit"]["0"];
		effects[true] = src["hit"]["1"];
	}

	void TwoHHitEffect::apply(Mob& target, uint16_t, bool twohanded) const
	{
		const Effect& effect = effects[twohanded];

		target.showeffect(effect.animation, effect.pos);
	}


	ByLevelHitEffect::ByLevelHitEffect(node src)
	{
		for (node sub : src["CharLevel"])
		{
			uint16_t level = StringConversion<uint16_t>(sub.name()).orzero();
			effects[level] = sub["hit"]["0"];
		}
	}

	void ByLevelHitEffect::apply(Mob& target, uint16_t level, bool) const
	{
		uint16_t max = 0;
		for (auto& hit : effects)
		{
			uint16_t required = hit.first;
			if (level < required)
			{
				break;
			}
			else
			{
				max = required;
			}
		}

		if (effects.count(max))
		{
			const Effect& effect = effects.at(max);

			target.showeffect(effect.animation, effect.pos);
		}
	}


	ByLevelTwoHHitEffect::ByLevelTwoHHitEffect(node src)
	{
		for (node sub : src["CharLevel"])
		{
			uint16_t level = StringConversion<uint16_t>(sub.name()).orzero();
			effects[level][false] = sub["hit"]["0"];
			effects[level][true] = sub["hit"]["1"];
		}
	}

	void ByLevelTwoHHitEffect::apply(Mob& target, uint16_t level, bool twohanded) const
	{
		uint16_t max = 0;
		for (auto& hit : effects)
		{
			uint16_t required = hit.first;
			if (level < required)
			{
				break;
			}
			else
			{
				max = required;
			}
		}

		if (effects.count(max))
		{
			const Effect& effect = effects.at(max)[twohanded];

			target.showeffect(effect.animation, effect.pos);
		}
	}
}