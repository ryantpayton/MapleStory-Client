//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../Template/Enumeration.h"
#include <cstdint>
#include <cstddef>

namespace ms
{
	namespace EquipStat
	{
		enum Id
		{
			STR,
			DEX,
			INT,
			LUK,
			HP,
			MP,
			WATK,
			MAGIC,
			WDEF,
			MDEF,
			ACC,
			AVOID,
			HANDS,
			SPEED,
			JUMP,
			LENGTH
		};

		Id by_id(size_t id);

		int32_t value_of(Id value);

		constexpr Enumeration<Id> values;
		extern const char *names[Id::LENGTH];
	};
}
