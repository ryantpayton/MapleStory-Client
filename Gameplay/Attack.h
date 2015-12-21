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
		Util::vector2d<int16_t> origin;
		Util::rectangle2d<int16_t> range;

		Graphics::Animation hiteffect;
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