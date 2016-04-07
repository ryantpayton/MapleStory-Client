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
#include "EffectLayer.h"

namespace Graphics
{
	void EffectLayer::drawbelow(Point<int16_t> position, float alpha) const
	{
		for (auto iter = effects.begin(); iter != effects.upper_bound(-1); ++iter)
		{
			for (auto& effect : iter->second)
			{
				effect.draw(position, alpha);
			}
		}
	}

	void EffectLayer::drawabove(Point<int16_t> position, float alpha) const
	{
		for (auto iter = effects.upper_bound(-1); iter != effects.end(); ++iter)
		{
			for (auto& effect : iter->second)
			{
				effect.draw(position, alpha);
			}
		}
	}

	void EffectLayer::update()
	{
		for (auto& effectlist : effects)
		{
			effectlist.second.remove_if([](Effect& effect){
				return effect.update();
			});
		}
	}

	void EffectLayer::add(Animation animation, DrawArgument args, int8_t z, float speed)
	{
		effects[z].emplace_back(animation, args, speed);
	}

	void EffectLayer::add(Animation animation, DrawArgument args, int8_t z)
	{
		add(animation, args, z, 1.0f);
	}

	void EffectLayer::add(Animation animation, DrawArgument args)
	{
		add(animation, args, 0, 1.0f);
	}

	void EffectLayer::add(Animation animation)
	{
		add(animation, DrawArgument(), 0, 1.0f);
	}
}