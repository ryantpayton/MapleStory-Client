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
#include "SkillUseEffect.h"

#include "Util\Misc.h"

namespace Gameplay
{
	SingleUseEffect::SingleUseEffect(node src)
	{
		effect = src["effect"];
	}

	void SingleUseEffect::apply(Char& target) const
	{
		target.showeffect(effect.animation, effect.z);
	}


	TwoHUseEffect::TwoHUseEffect(node src)
	{
		effects[false] = src["effect"]["0"];
		effects[true] = src["effect"]["1"];
	}

	void TwoHUseEffect::apply(Char& target) const
	{
		bool twohanded = target.istwohanded();
		const Effect& effect = effects[twohanded];

		target.showeffect(effect.animation, effect.z);
	}


	MultiUseEffect::MultiUseEffect(node src)
	{
		int8_t no = -1;
		node sub = src["effect"];
		while (sub)
		{
			effects.push_back(sub);

			no++;
			sub = src["effect" + std::to_string(no)];
		}
	}

	void MultiUseEffect::apply(Char& target) const
	{
		for (auto& efit : effects)
		{
			target.showeffect(efit.animation, efit.z);
		}
	}


	ByLevelUseEffect::ByLevelUseEffect(node src)
	{
		for (node sub : src["CharLevel"])
		{
			uint16_t level = StringConversion<uint16_t>(sub.name()).orzero();
			leveleffects[level] = sub["effect"];
		}
	}

	void ByLevelUseEffect::apply(Char& target) const
	{
		uint16_t level = target.getlevel();
		uint16_t max = 0;
		for (auto& effect : leveleffects)
		{
			uint16_t required = effect.first;
			if (level < required)
			{
				break;
			}
			else
			{
				max = required;
			}
		}

		if (leveleffects.count(max))
		{
			const Effect& effect = leveleffects.at(max);
			target.showeffect(effect.animation, effect.z);
		}
	}
}