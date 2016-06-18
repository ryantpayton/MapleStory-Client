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
#include "Equipslot.h"

#include "..\..\Console.h"
#include "..\..\Util\Literals.h"

namespace jrc
{
	Equipslot::Value Equipslot::byid(size_t id)
	{
		if (id >= LENGTH)
		{
			Console::get()
				.print("Invalid Equipslot id: " + std::to_string(id));
			return NONE;
		}
		return static_cast<Value>(id);
	}

	Equipslot::Value Equipslot::byvalue(int16_t v)
	{
		for (auto iter : values)
		{
			if (iter.second == v)
				return iter.first;
		}

		Console::get()
			.print("Invalid equip slot value: " + std::to_string(v));
		return NONE;
	}


	const EnumMap<Equipslot::Value, int16_t> Equipslot::values =
	{
		0_s, 1_s, 2_s, 3_s, 4_s, 5_s, 6_s,
		7_s, 8_s, 9_s, 10_s, 11_s, 12_s, 13_s,
		15_s, 16_s, 17_s, 18_s, 19_s, 49_s, 50_s
	};
}