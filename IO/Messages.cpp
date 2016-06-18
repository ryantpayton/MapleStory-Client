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
#include "Messages.h"
#include "UI.h"

#include "UITypes\UIStatusbar.h"

namespace jrc
{
	Messages::Messages()
	{
		messages[NONE] = "";

		// Cannot use a skill
		messages[SKILL_WEAPONTYPE] = "You cannot use this skill with this weapon.";
		messages[SKILL_HPCOST] = "You do not have enough hp to use this skill.";
		messages[SKILL_MPCOST] = "You do not have enough mp to use this skill.";
		messages[SKILL_NOARROWS] = "You do not have enough arrows to use this attack.";
		messages[SKILL_NOBULLETS] = "You do not have enough bullets to use this attack.";
		messages[SKILL_NOSTARS] = "You do not have enough throwing stars to use this attack.";
		messages[SKILL_COOLDOWN] = "You cannot use this skill as it is on cooldown.";

		// Scrolling result
		messages[SCROLL_SUCCESS] = "The scroll lights up and it's mysterious powers have been transferred to the item.";
		messages[SCROLL_FAILURE] = "The scroll lights up but the item remains as if nothing happened.";
		messages[SCROLL_DESTROYED] = "The item has been destroyed due to the overwhelming power of the scroll.";
	}

	std::string Messages::stringfor(Type type)
	{
		return messages[type];
	}


	InChatMessage::InChatMessage(Messages::Type t)
	{
		type = t;
	}

	void InChatMessage::drop() const
	{
		if (type == Messages::NONE)
			return;

		UI::get().get_element<UIStatusbar>()
			.if_present(&UIStatusbar::display_message, type, UIChatbar::RED);
	}


	ForbidSkillMessage::ForbidSkillMessage(SpecialMove::ForbidReason reason, Weapon::Type weapon)
		: InChatMessage(message_by_reason(reason, weapon)) {}

	Messages::Type ForbidSkillMessage::message_by_reason(SpecialMove::ForbidReason reason, Weapon::Type weapon)
	{
		switch (reason)
		{
		case SpecialMove::FBR_WEAPONTYPE:
			return Messages::SKILL_WEAPONTYPE;
		case SpecialMove::FBR_HPCOST:
			return Messages::SKILL_HPCOST;
		case SpecialMove::FBR_MPCOST:
			return Messages::SKILL_MPCOST;
		case SpecialMove::FBR_COOLDOWN:
			return Messages::SKILL_COOLDOWN;
		case SpecialMove::FBR_BULLETCOST:
			return message_by_weapon(weapon);
		default:
			return Messages::NONE;
		}
	}

	Messages::Type ForbidSkillMessage::message_by_weapon(Weapon::Type weapon)
	{
		switch (weapon)
		{
		case Weapon::BOW:
		case Weapon::CROSSBOW:
			return Messages::SKILL_NOARROWS;
		case Weapon::CLAW:
			return Messages::SKILL_NOSTARS;
		default:
			return Messages::SKILL_NOBULLETS;
		}
	}
}