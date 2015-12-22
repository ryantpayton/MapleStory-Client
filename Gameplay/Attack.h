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
#pragma once
#include <cstdint>
#include "Skill.h"
#include "Util\rectangle2d.h"
#include "Graphics\Animation.h"

namespace Gameplay
{
	using std::uint8_t;
	using std::int16_t;
	using std::uint16_t;
	using std::int32_t;
	using std::vector;
	using std::map;
	using Util::vector2d;
	using Util::rectangle2d;
	using Graphics::Animation;

	struct Attack 
	{
		enum Direction
		{
			CENTERED,
			TOLEFT,
			TORIGHT
		};

		int32_t mindamage = 0;
		int32_t maxdamage = 0;
		float critical = 0.0f;
		float ignoredef = 0.0f;
		int32_t accuracy = 0;
		int16_t playerlevel = 1;

		uint8_t delay = 0;
		uint8_t hitcount = 0;
		uint8_t mobcount = 0;

		int32_t skill = 0;
		uint8_t speed = 0;

		Direction direction = CENTERED;
		vector2d<int16_t> origin;
		rectangle2d<int16_t> range;

		Animation hiteffect;

		void applyskill(int32_t skillid, Animation effect, const SkillLevel& level)
		{
			skill = skillid;
			hiteffect = effect;
			hitcount = level.attackcount;
			mobcount = level.mobcount;
			range = level.range;
			critical += level.critical;
			ignoredef += level.ignoredef;
			mindamage = static_cast<int32_t>(mindamage * level.damage);
			maxdamage = static_cast<int32_t>(maxdamage * level.damage);
		}
	};

	struct AttackResult
	{
		map<int32_t, vector<int32_t>> damagelines;
		uint8_t mobcount = 0;
		uint8_t hitcount = 1;
		int32_t skill = 0;
		int32_t charge = 0;
		uint8_t display = 0;
		uint8_t direction = 0;
		uint8_t stance = 0;
		uint8_t speed = 0;
	};
}