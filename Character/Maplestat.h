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
#include "Util\Misc.h"

namespace Character
{
	class Maplestat
	{
	public:
		static const size_t LENGTH = 20;
		enum Value
		{
			SKIN, FACE, HAIR, LEVEL, JOB,
			STR, DEX, INT, LUK,
			HP, MAXHP, MP, MAXMP,
			AP, SP, EXP, FAME, MESO,
			PET, GACHAEXP
		};

		static EnumIterator<Value> it(Value s = SKIN, Value l = GACHAEXP)
		{
			return EnumIterator<Value>(s, l);
		}

		static Value byid(size_t id)
		{
			return static_cast<Value>(id);
		}

		static int32_t valueof(Value v)
		{
			static const int32_t values[LENGTH] =
			{
				0x1, 0x2, 0x4, 0x10, 0x20,
				0x40, 0x80, 0x100, 0x200, 
				0x400, 0x800, 0x1000, 0x2000, 
				0x4000, 0x8000, 0x10000,  0x20000, 0x40000, 
				0x180008, 0x200000
			};
			return values[v];
		}

		static bool compare(Value first, int32_t second)
		{
			return Bits::compare(valueof(first), second);
		}
	};
}