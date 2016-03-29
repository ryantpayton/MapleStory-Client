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
#include "Constants.h"

namespace Graphics
{
	void EffectLayer::drawbelow(Point<int16_t> position, float alpha) const
	{
		for (auto& effectlist : effectsbelow)
		{
			for (auto& effect : effectlist.second)
			{
				effect.draw(position, alpha);
			}
		}
	}

	void EffectLayer::drawabove(Point<int16_t> position, float alpha) const
	{
		for (auto& effectlist : effectsabove)
		{
			for (auto& effect : effectlist.second)
			{
				effect.draw(position, alpha);
			}
		}
	}

	void EffectLayer::update()
	{
		for (auto& effectlist : effectsbelow)
		{
			effectlist.second.remove_if([](Effect& effect){
				return effect.update();
			});
		}

		for (auto& effectlist : effectsabove)
		{
			effectlist.second.remove_if([](Effect& effect){
				return effect.update();
			});
		}
	}

	void EffectLayer::add(Animation animation, bool flip, int8_t z, float speed)
	{
		auto timestep = static_cast<uint16_t>(Constants::TIMESTEP * speed);
		auto effect = Effect(animation, timestep, flip);
		bool below = z < 0;
		if (below)
		{
			effectsbelow[z].push_back(effect);
		}
		else
		{
			effectsabove[z].push_back(effect);
		}
	}

	void EffectLayer::add(Animation animation, bool flip, int8_t z)
	{
		add(animation, flip, z, 1.0f);
	}

	void EffectLayer::add(Animation animation, bool flip)
	{
		add(animation, flip, 0, 1.0f);
	}

	void EffectLayer::add(Animation animation)
	{
		add(animation, false, 0, 1.0f);
	}
}