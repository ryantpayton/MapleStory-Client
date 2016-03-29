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
#include "SkillAction.h"

namespace Gameplay
{
	using Character::Weapon;

	void RegularAction::apply(Char& target, Attack::Type atype) const
	{
		Weapon::Type weapontype = target.getlook().getequips().getweapontype();
		bool degenerate;
		switch (weapontype)
		{
		case Weapon::BOW:
		case Weapon::CROSSBOW:
		case Weapon::CLAW:
		case Weapon::GUN:
			degenerate = atype != Attack::RANGED;
			break;
		default:
			degenerate = false;
		}

		target.attack(degenerate);
	}


	SingleAction::SingleAction(node src)
	{
		action = src["action"]["0"];
	}

	void SingleAction::apply(Char& target, Attack::Type) const
	{
		target.attack(action);
	}


	TwoHAction::TwoHAction(node src)
	{
		actions[false] = src["action"]["0"];
		actions[true] = src["action"]["1"];
	}

	void TwoHAction::apply(Char& target, Attack::Type) const
	{
		bool twohanded = target.istwohanded();
		string action = actions[twohanded];

		target.attack(action);
	}


	ByLevelAction::ByLevelAction(node src, int32_t id)
	{
		for (node sub : src["level"])
		{
			int32_t level = StringConversion<int32_t>(sub.name()).orzero();
			actions[level] = sub["action"];
		}

		skillid = id;
	}

	void ByLevelAction::apply(Char& target, Attack::Type) const
	{
		int32_t level = target.getskilllevel(skillid);
		if (actions.count(level))
		{
			string action = actions.at(level);
			target.attack(action);
		}
	}
}