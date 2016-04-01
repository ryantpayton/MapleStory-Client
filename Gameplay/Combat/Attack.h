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
#include "Audio\Audio.h"
#include "Character\Look\Stance.h"
#include "Graphics\Animation.h"
#include "Util\rectangle2d.h"
#include <cstdint>

namespace Gameplay
{
	using std::vector;
	using std::map;
	using Graphics::Animation;
	using Audio::Sound;

	struct Attack 
	{
		enum Type
		{
			CLOSE,
			RANGED,
			MAGIC
		};

		enum Direction : uint8_t
		{
			CENTERED = 0,
			TOLEFT = 1,
			TORIGHT = 2
		};

		Type type = CLOSE;

		double mindamage = 1.0;
		double maxdamage = 1.0;
		float critical = 0.0f;
		float ignoredef = 0.0f;
		int32_t accuracy = 0;
		int16_t playerlevel = 1;

		uint8_t hitcount = 0;
		uint8_t mobcount = 0;
		uint8_t speed = 0;
		uint8_t stance = 0;
		int32_t skill = 0;
		int32_t bullet = 0;

		float hrange = 1.0f;
		uint16_t delay = 0;
		Direction direction = CENTERED;
		vector<uint16_t> hitdelays;
		Point<int16_t> origin;
		rectangle2d<int16_t> range;
	};


	struct AttackResult
	{
		AttackResult(const Attack& attack)
		{
			type = attack.type;
			hitcount = attack.hitcount;
			skill = attack.skill;
			direction = attack.direction;
			speed = attack.speed;
			stance = attack.stance;
			bullet = attack.bullet;
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
		uint8_t direction = 0;
		uint8_t stance = 0;
		uint8_t speed = 0;
		map<int32_t, vector<pair<int32_t, bool>>> damagelines;
	};
}