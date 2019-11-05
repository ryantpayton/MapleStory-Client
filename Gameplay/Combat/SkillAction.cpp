//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "SkillAction.h"

#include "../../Util/Misc.h"

namespace ms
{
	void RegularAction::apply(Char& target, Attack::Type atype) const
	{
		Weapon::Type weapontype = target.get_weapontype();
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
			break;
		}

		target.attack(degenerate);
	}

	SingleAction::SingleAction(nl::node src)
	{
		action = src["action"]["0"].get_string();
	}

	void SingleAction::apply(Char& target, Attack::Type) const
	{
		target.attack(action);
	}

	TwoHAction::TwoHAction(nl::node src)
	{
		actions[false] = src["action"]["0"].get_string();
		actions[true] = src["action"]["1"].get_string();
	}

	void TwoHAction::apply(Char& target, Attack::Type) const
	{
		bool twohanded = target.is_twohanded();
		std::string action = actions[twohanded];

		target.attack(action);
	}

	ByLevelAction::ByLevelAction(nl::node src, std::int32_t id)
	{
		for (auto sub : src["level"])
		{
			std::int32_t level = string_conversion::or_zero<std::int32_t>(sub.name());
			actions[level] = sub["action"].get_string();
		}

		skillid = id;
	}

	void ByLevelAction::apply(Char& target, Attack::Type) const
	{
		std::int32_t level = target.get_skilllevel(skillid);
		auto iter = actions.find(level);

		if (iter != actions.end())
			target.attack(iter->second);
	}
}