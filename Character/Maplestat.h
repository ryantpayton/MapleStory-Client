/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "..\Util\EnumMap.h"

#include <cstdint>

namespace jrc
{
	class Maplestat
	{
	public:
		enum Value
		{
			SKIN, FACE, HAIR, LEVEL, JOB,
			STR, DEX, INT, LUK,
			HP, MAXHP, MP, MAXMP,
			AP, SP, EXP, FAME, MESO,
			PET, GACHAEXP,
			LENGTH
		};

		static Value byid(size_t id);

		static const EnumMap<Value, int32_t> values;
	};
}