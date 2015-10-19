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
#include "EquipData.h"

namespace Character
{
	enum PotentialRank
	{
		PTR_NONE,
		PTR_HIDDEN,
		PTR_RARE,
		PTR_EPIC,
		PTR_UNIQUE,
		PTR_LEGEND
	};

	enum EquipQuality
	{
		EQQ_GREY,
		EQQ_WHITE,
		EQQ_ORANGE,
		EQQ_BLUE,
		EQQ_VIOLET,
		EQQ_GOLD
	};

	class Equip : public Item
	{
	public:
		Equip(const ItemData&, int, bool, int64_t, int64_t, char, char, map<Equipstat, short>, string, short, char, short, int);
		~Equip();
	private:
		char slots;
		char level;
		map<Equipstat, short> stats;
		char itemlevel;
		short itemexp;
		int vicious;
		PotentialRank rank;
		EquipQuality quality;
	};
}

