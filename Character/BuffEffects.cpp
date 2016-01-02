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
#include "BuffEffects.h"
#include <unordered_map>
#include <memory>

namespace Character
{
	BuffEffect* ebsswitch(Buffstat stat)
	{
		switch (stat)
		{
		case BOOSTER:
			return new BoosterEffect();
		case HYPERBODYHP:
			return new HyperbodyHPEffect();
		case HYPERBODYMP:
			return new HyperbodyHPEffect();
		default:
			return nullptr;
		}
	}

	const BuffEffect* geteffectbystat(Buffstat stat)
	{
		using std::unordered_map;
		using std::unique_ptr;

		static unordered_map<Buffstat, unique_ptr<BuffEffect>> buffeffects;
		if (!buffeffects.count(stat))
		{
			BuffEffect* neweffect = ebsswitch(stat);
			if (neweffect == nullptr)
				return nullptr;
			else
				buffeffects[stat] = unique_ptr<BuffEffect>(neweffect);
		}

		return buffeffects[stat].get();
	}
}