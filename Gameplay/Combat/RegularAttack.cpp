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
#include "RegularAttack.h"

namespace jrc
{
	RegularAttack::RegularAttack()
	{
		action = std::make_unique<RegularAction>();
		bullet = std::make_unique<RegularBullet>();
		sound = std::make_unique<NoSkillSound>();
		useeffect = std::make_unique<NoUseEffect>();
		hiteffect = std::make_unique<NoHitEffect>();
	}

	void RegularAttack::applystats(const Char& user, Attack& attack) const
	{
		attack.damagetype = Attack::DMG_WEAPON;
		attack.skill = 0;
		attack.mobcount = 1;
		attack.hitcount = 1;
		attack.stance = user.getlook().getattacklook().stance;
		if (attack.type == Attack::CLOSE)
		{
			attack.range = user.getlook().getattacklook().range;
		}
	}

	bool RegularAttack::isoffensive() const
	{
		return true;
	}

	int32_t RegularAttack::getid() const
	{
		return 0;
	}

	SpecialMove::ForbidReason RegularAttack::canuse(int32_t, Weapon::Type weapon, 
		const CharJob&, uint16_t, uint16_t, uint16_t bullets) const {

		switch (weapon)
		{
		case Weapon::BOW:
		case Weapon::CROSSBOW:
		case Weapon::CLAW:
		case Weapon::GUN:
			return bullets ? FBR_NONE : FBR_BULLETCOST;
		default:
			return FBR_NONE;
		}
	}
}