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
#include "SeededState.h"
#include <stdexcept>

namespace Net
{
	SeededState::SeededState(int32_t seed, uint8_t nms, uint8_t st)
	{
		for (int32_t i = 0; i < 4; i++)
		{
			value[i] = static_cast<uint8_t>(seed);
			seed = seed >> 8;
		}

		numstates = nms;
		state = st;
	}

	SeededState::SeededState() {}

	SeededState::~SeededState() {}

	int32_t SeededState::getvalue()
	{
		int32_t all = 0;
		for (int32_t i = 0; i < 4; i++)
		{
			all += value[i] << (8 * i);
		}
		return all;
	}

	void SeededState::nextstate()
	{
		static uint8_t bytes[4] = 
		{ 
			69, 42, 13, 124 
		};

		for (int32_t i = 0; i < 4; i++)
		{
			value[i] = bytes[i] + bytes[value[3 - i] % 4] - value[i];
		}
		state = static_cast<uint8_t>(getvalue()) % numstates;
	}

	uint8_t SeededState::getstate()
	{
		return state;
	}
}