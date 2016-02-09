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
#include "Buff.h"
#include "BuffEffects.h"

namespace Character
{
	Buff::Buff(Buffstat::Value s, int16_t v, int32_t i, int32_t d)
	{
		stat = s;
		value = v;
		skillid = i;
		duration = d;
	}

	Buff::Buff() {}

	Buff::~Buff() {}

	void Buff::applyto(CharStats& stats) const
	{
		const BuffEffect* effect = BuffEffects::get().bystat(stat);
		if (effect)
			effect->applyto(value, stats);
	}

	Buffstat::Value Buff::getstat() const
	{
		return stat;
	}
}