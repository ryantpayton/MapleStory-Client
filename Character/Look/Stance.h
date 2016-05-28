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
#include "..\..\Console.h"
#include "..\..\Util\Enum.h"

#include <cstdint>
#include <string>

namespace jrc
{
	class Stance
	{
	public:
		enum Value : uint8_t
		{
			NONE, ALERT, DEAD, FLY, HEAL, JUMP, LADDER, PRONE, PRONESTAB,
			ROPE, SHOT, SHOOT1, SHOOT2, SHOOTF, SIT, STABO1, STABO2, STABOF,
			STABT1, STABT2, STABTF, STAND1, STAND2, SWINGO1, SWINGO2,
			SWINGO3, SWINGOF, SWINGP1, SWINGP2, SWINGPF, SWINGT1, SWINGT2,
			SWINGT3, SWINGTF, WALK1, WALK2,
			LENGTH
		};

		static EnumIterator<Value> getit()
		{
			return EnumIterator<Value>(ALERT, WALK2);
		}

		static Value bystate(int8_t state)
		{
			int8_t index = (state / 2) - 1;
			if (index < 0 || index > 10)
				return WALK1;

			static const Value statevalues[10] =
			{
				WALK1, STAND1, JUMP, ALERT,
				PRONE, FLY, LADDER, ROPE,
				DEAD, SIT
			};
			return statevalues[index];
		}

		static Value byid(uint8_t id)
		{
			for (auto it = getit(); it.hasnext(); it.increment())
			{
				Stance::Value value = it.get();
				if (id == value)
					return value;
			}
			return NONE;
		}

		static Value bystring(std::string name)
		{
			for (auto it = getit(); it.hasnext(); it.increment())
			{
				Value value = it.get();
				if (nameof(value) == name)
					return value;
			}

			Console::get().print("Unhandled stance: " + name);

			return NONE;
		}

		static std::string nameof(Value value)
		{
			static const std::string stancenames[LENGTH] =
			{
				"", "alert", "dead", "fly", "heal", "jump", "ladder", "prone", "proneStab",
				"rope", "shot", "shoot1", "shoot2", "shootF", "sit", "stabO1", "stabO2", "stabOF",
				"stabT1", "stabT2", "stabTF", "stand1", "stand2", "swingO1", "swingO2",
				"swingO3", "swingOF", "swingP1", "swingP2", "swingPF", "swingT1", "swingT2",
				"swingT3", "swingTF", "walk1", "walk2"
			};
			return stancenames[value];
		}

		static bool isclimbing(Value value)
		{
			return value == LADDER || value == ROPE;
		}

		static Value baseof(Value value)
		{
			switch (value)
			{
			case STAND2:
				return STAND1;
			case WALK2:
				return WALK1;
			default:
				return value;
			}
		}

		static Value secondof(Value value)
		{
			switch (value)
			{
			case STAND1:
				return STAND2;
			case WALK1:
				return WALK2;
			default:
				return value;
			}
		}
	};
}