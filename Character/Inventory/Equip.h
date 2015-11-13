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
#include "Item.h"
#include "Character\EquipConstants.h"

namespace Character
{
	using::std::map;

	class Equip : public Item
	{
	public:
		enum Potential;
		enum Quality;

		Equip(const ItemData&, int32_t, bool, int64_t, int64_t, uint8_t, uint8_t, 
			map<Equipstat, uint16_t>, string, int16_t, uint8_t, int16_t, int32_t);
		~Equip();

		uint16_t getstat(Equipstat type) const;

	private:
		uint8_t slots;
		uint8_t level;
		map<Equipstat, uint16_t> stats;
		uint8_t itemlevel;
		int16_t itemexp;
		int32_t vicious;
		Potential potrank;
		Quality quality;
	};
}

