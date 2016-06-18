//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "SpecialMove.h"

namespace jrc
{
	void SpecialMove::apply_useeffects(Char& target, Attack::Type type) const
	{
		useeffect->apply(target);
		action->apply(target, type);

		sound->play_use();
	}

	void SpecialMove::apply_hiteffects(const AttackUser& user, Mob& target) const
	{
		hiteffect->apply(user, target);

		sound->play_hit();
	}

	bool SpecialMove::isskill() const
	{
		return get_id() > 0;
	}

	Animation SpecialMove::get_animation(const Char& user, int32_t bulletid) const
	{
		return bullet->get(user, bulletid);
	}
}