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
	EffectLayer::EffectLayer() 
	{
		reid = 0;
	}

	EffectLayer::~EffectLayer() {}

	void EffectLayer::draw(vector2d<int16_t> position, float inter) const
	{
		for (auto& eff : effects)
		{
			eff.second.draw(DrawArgument(position), inter);
		}
	}

	void EffectLayer::update()
	{
		vector<uint8_t> toremove;
		for (auto& eff : effects)
		{
			bool expired = eff.second.update();
			if (expired)
				toremove.push_back(eff.first);
		}

		for (auto& rm : toremove)
		{
			effects.erase(rm);
		}
	}

	void EffectLayer::add(Animation animation)
	{
		effects[reid] = animation;
		reid++;
	}
}