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
	using std::unordered_map;
	using std::unique_ptr;

	unordered_map<Buffstat::Value, unique_ptr<BuffEffect>> buffeffects;

	void initbuffeffects()
	{
		buffeffects[Buffstat::MAPLE_WARRIOR] = unique_ptr<BuffEffect>(new MapleWarriorEffect());
		buffeffects[Buffstat::STANCE] = unique_ptr<BuffEffect>(new StanceEffect());
		buffeffects[Buffstat::BOOSTER] = unique_ptr<BuffEffect>(new BoosterEffect());
		buffeffects[Buffstat::WATK] = unique_ptr<BuffEffect>(new WATKEffect());
		buffeffects[Buffstat::WDEF] = unique_ptr<BuffEffect>(new WDEFEffect());
		buffeffects[Buffstat::MATK] = unique_ptr<BuffEffect>(new MATKEffect());
		buffeffects[Buffstat::MDEF] = unique_ptr<BuffEffect>(new MDEFEffect());
		buffeffects[Buffstat::SPEED] = unique_ptr<BuffEffect>(new SPEEDEffect());
		buffeffects[Buffstat::JUMP] = unique_ptr<BuffEffect>(new JUMPEffect());
		buffeffects[Buffstat::HYPERBODYHP] = unique_ptr<BuffEffect>(new HyperbodyHPEffect());
		buffeffects[Buffstat::HYPERBODYMP] = unique_ptr<BuffEffect>(new HyperbodyMPEffect());
	}

	const BuffEffect* geteffectbystat(Buffstat::Value stat)
	{
		if (buffeffects.count(stat))
			return buffeffects[stat].get();
		else
			return nullptr;
	}
}