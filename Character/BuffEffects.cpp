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

namespace jrc
{
	BuffEffects::BuffEffects()
	{
		buffeffects[Buffstat::MAPLE_WARRIOR] = std::make_unique<MapleWarriorEffect>();
		buffeffects[Buffstat::STANCE] = std::make_unique<StanceEffect>();
		buffeffects[Buffstat::BOOSTER] = std::make_unique<BoosterEffect>();
		buffeffects[Buffstat::WATK] = std::make_unique<WATKEffect>();
		buffeffects[Buffstat::WDEF] = std::make_unique<WDEFEffect>();
		buffeffects[Buffstat::MATK] = std::make_unique<MATKEffect>();
		buffeffects[Buffstat::MDEF] = std::make_unique<MDEFEffect>();
		buffeffects[Buffstat::SPEED] = std::make_unique<SPEEDEffect>();
		buffeffects[Buffstat::JUMP] = std::make_unique<JUMPEffect>();
		buffeffects[Buffstat::HYPERBODYHP] = std::make_unique<HyperbodyHPEffect>();
		buffeffects[Buffstat::HYPERBODYMP] = std::make_unique<HyperbodyMPEffect>();
	}

	const BuffEffect* BuffEffects::bystat(Buffstat::Value stat) const
	{
		return buffeffects.count(stat) ? buffeffects.at(stat).get() : nullptr;
	}
}