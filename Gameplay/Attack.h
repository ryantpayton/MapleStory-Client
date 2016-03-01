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
#include "Util\rectangle2d.h"
#include "Graphics\Animation.h"
#include "Audio\Sound.h"
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

		int32_t skill = 0;
		uint8_t speed = 0;

		uint16_t delay = 0;
		Direction direction = CENTERED;
		Point<int16_t> origin;
		rectangle2d<int16_t> range;

		Animation bullet;
		Animation hiteffect;
		Sound usesound;
		Sound hitsound;

		bool update(uint16_t timestep)
		{
			if (delay < timestep)
				return true;

			delay -= timestep;
			return false;
		}
	};

	struct AttackResult
	{
		AttackResult(const Attack& attack)
		{
			hitcount = attack.hitcount;
			skill = attack.skill;
			direction = attack.direction;
			speed = attack.speed;
			ranged = attack.type == Attack::RANGED;
		}

		bool ranged = false;
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