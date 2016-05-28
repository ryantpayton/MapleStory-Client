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
#include "..\Util\Enum.h"

namespace jrc
{
	class Equipstat
	{
	public:
		enum Value
		{
			STR, DEX, INT, LUK, HP, MP,
			WATK, MAGIC, WDEF, MDEF,
			ACC, AVOID, HANDS, SPEED, JUMP,
			LENGTH
		};

		static Value byid(size_t id)
		{
			return static_cast<Value>(id);
		}

		static int32_t valueof(Value value)
		{
			return value;
		}

		static constexpr char* names[LENGTH] =
		{
			"STR", "DEX", "INT", "LUK", "MAX HP", "MAX MP",
			"WEAPON ATT", "MAGIC ATT", "WEAPON DEFENSE", "MAGIC DEFENSE",
			"ACCURACY", "AVOID", "HANDS", "SPEED", "JUMP"
		};
	};
}