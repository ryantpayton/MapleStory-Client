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
#include <vector>

namespace Gameplay
{
	using std::uint8_t;
	using std::int16_t;
	using std::uint16_t;

	// Contains information on a single movement.
	struct MovementFragment
	{
		// Movement types for servers and other clients to handle.
		enum MovementType
		{
			MVT_NONE,
			MVT_ABSOLUTE,
			MVT_RELATIVE,
			MVT_CHAIR,
			MVT_JUMPDOWN
		};

		MovementType type = MVT_NONE;
		uint8_t command = 0;
		int16_t xpos = 0;
		int16_t ypos = 0;
		int16_t lastx = 0;
		int16_t lasty = 0;
		uint16_t fh = 0;
		uint8_t newstate = 0;
		int16_t duration = 0;
	};
}