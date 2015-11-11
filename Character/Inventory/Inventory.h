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
#include "Character\Inventory\Item.h"
#include "Character\Inventory\Pet.h"
#include "Character\Inventory\Equip.h"
#include "Util\Ptrmap.h"

namespace Character
{
	using::Util::Ptrmap;

	class Inventory
	{
	public:
		enum InvType
		{
			EQUIPPED = -1,
			NONE = 0,
			EQUIP = 1,
			USE = 2,
			SETUP = 3,
			ETC = 4,
			CASH = 5
		};

		Inventory();
		~Inventory();
		void setmeso(int64_t);
		void setslots(InvType, uint8_t);
		void additem(InvType, int16_t, int32_t, bool,
			int64_t, int64_t, uint16_t, string, int16_t);
		void addpet(InvType, int16_t, int32_t, bool,
			int64_t, int64_t, string, int8_t, int16_t, int8_t);
		void addequip(InvType, int16_t, int32_t, bool, int64_t, int64_t, uint8_t,
			uint8_t, map<Equipstat, uint16_t>, string, int16_t, uint8_t, uint16_t, int32_t);
	private:
		int64_t meso;
		map<InvType, uint8_t> slots;
		map<InvType, Ptrmap<int16_t, Item>> inventoryitems;
	};
}

