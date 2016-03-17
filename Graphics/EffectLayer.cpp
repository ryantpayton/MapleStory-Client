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
	void EffectLayer::draw(Point<int16_t> position, float alpha) const
	{
		for (auto& effect : effects)
		{
			effect.draw(position, alpha);
		}
	}

	void EffectLayer::update()
	{
		effects.remove_if([](Effect& effect){
			return effect.animation.update();
		});
	}

	void EffectLayer::add(Animation animation, bool flip)
	{
		effects.push_back({ animation, flip });
	}

	void EffectLayer::add(Animation animation)
	{
		add(animation, false);
	}
}