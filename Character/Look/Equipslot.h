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
#include "Util\Enum.h"
#include "Console.h"
#include <cstdint>

namespace Character
{
	class Equipslot
	{
	public:
		static const size_t LENGTH = 21;
		enum Value
		{
			NONE, CAP, FACEACC, EYEACC, EARRINGS, TOP, PANTS,
			SHOES, GLOVES, CAPE, SHIELD, WEAPON, RING, RING2,
			RING3, RING4, PENDANT, TAMEDMOB, SADDLE, MEDAL, BELT
		};

		static EnumIterator<Value> getit(Value s = CAP, Value l = BELT)
		{
			return EnumIterator<Value>(s, l);
		}

		static Value byid(size_t id)
		{
			return static_cast<Value>(id);
		}

		static Value byvalue(int16_t v)
		{
			for (auto it = getit(); it.hasnext(); it.increment())
			{
				Value value = it.get();
				if (valueof(value) == v)
					return value;
			}

			Console::get().print("Unhandled equip slot value: " + std::to_string(v));

			return NONE;
		}

		static int16_t valueof(Value value)
		{
			static int16_t values[LENGTH] =
			{
				0, 1, 2, 3, 4, 5, 6, 
				7, 8, 9, 10, 11, 12, 13, 
				15, 16, 17, 18, 19, 49, 50
			};
			return values[value];
		}
	};
}