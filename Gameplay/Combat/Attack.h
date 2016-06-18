/////////////////////////////////////////////////////////////////////////////
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
#pragma once
#include "..\..\Util\Rectangle.h"

#include <cstdint>
#include <vector>
#include <unordered_map>

namespace jrc
{
	struct Attack 
	{
		enum Type
		{
			CLOSE,
			RANGED,
			MAGIC
		};

		enum DamageType
		{
			DMG_WEAPON,
			DMG_MAGIC,
			DMG_FIXED
		};

		Type type = CLOSE;
		DamageType damagetype = DMG_WEAPON;

		double mindamage = 1.0;
		double maxdamage = 1.0;
		float critical = 0.0f;
		float ignoredef = 0.0f;
		int32_t matk = 0;
		int32_t accuracy = 0;
		int32_t fixdamage = 0;
		int16_t playerlevel = 1;

		uint8_t hitcount = 0;
		uint8_t mobcount = 0;
		uint8_t speed = 0;
		uint8_t stance = 0;
		int32_t skill = 0;
		int32_t bullet = 0;

		Point<int16_t> origin;
		Rectangle<int16_t> range;
		float hrange = 1.0f;
		bool toleft = false;
	};


	struct AttackResult
	{
		AttackResult(const Attack& attack)
		{
			type = attack.type;
			hitcount = attack.hitcount;
			skill = attack.skill;
			speed = attack.speed;
			stance = attack.stance;
			bullet = attack.bullet;
			toleft = attack.toleft;
		}

		AttackResult() {}

		Attack::Type type;
		uint8_t mobcount = 0;
		uint8_t hitcount = 1;
		int32_t skill = 0;
		int32_t charge = 0;
		int32_t bullet = 0;
		uint8_t level = 0;
		uint8_t display = 0;
		uint8_t stance = 0;
		uint8_t speed = 0;
		bool toleft = false;
		std::unordered_map<int32_t, std::vector<std::pair<int32_t, bool>>> damagelines;

		int32_t get_first_oid() const
		{
			auto begin = damagelines.begin();
			return begin != damagelines.end() ? begin->first : 0;
		}

		int32_t get_last_oid() const
		{
			return damagelines.size() > 0 ? (--damagelines.end())->first : 0;
		}
	};

	struct AttackUser
	{
		int32_t skilllevel;
		uint16_t level;
		bool secondweapon;
		bool flip;
	};
}