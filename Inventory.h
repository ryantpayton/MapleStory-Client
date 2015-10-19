/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "Pet.h"
#include "Equip.h"

namespace Character
{
	enum InventoryType
	{
		IVT_EQUIPPED = -1,
		IVT_NONE = 0,
		IVT_EQUIP = 1,
		IVT_USE = 2,
		IVT_SETUP = 3,
		IVT_ETC = 4,
		IVT_CASH = 5
	};

	class Inventory
	{
	public:
		Inventory();
		~Inventory(){}
		void setmeso(int64_t);
		void setslots(InventoryType, uint8_t);
		void add(Item*, InventoryType, short);
	private:
		int64_t meso;
		map<InventoryType, uint8_t> slots;
		map<InventoryType, Ptrmap<short, Item>> items;
	};
}

