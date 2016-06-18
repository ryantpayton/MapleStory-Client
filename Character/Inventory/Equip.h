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
#include "Item.h"

#include "..\Look\Clothing.h"
#include "..\..\Util\EnumMap.h"

namespace jrc
{
	class Equip : public Item
	{
	public:
		enum Potential
		{
			POT_NONE,
			POT_HIDDEN,
			POT_RARE,
			POT_EPIC,
			POT_UNIQUE,
			POT_LEGENDARY,
			LENGTH
		};

		enum Quality
		{
			EQQ_GREY,
			EQQ_WHITE,
			EQQ_ORANGE,
			EQQ_BLUE,
			EQQ_VIOLET,
			EQQ_GOLD
		};

		Equip(const ItemData&, int32_t, bool, int64_t, int64_t, uint8_t, uint8_t, 
			const EnumMap<Equipstat::Value, uint16_t>&, const std::string&, int16_t, uint8_t, int16_t, int32_t);

		uint8_t get_slots() const;
		uint8_t get_level() const;
		uint8_t getitemlevel() const;
		uint16_t get_stat(Equipstat::Value type) const;
		int32_t getvicious() const;
		Potential getpotrank() const;
		Quality getquality() const;

		const Clothing& getcloth() const;

	private:
		void checkquality();

		EnumMap<Equipstat::Value, uint16_t> stats;
		uint8_t slots;
		uint8_t level;
		uint8_t itemlevel;
		int16_t itemexp;
		int32_t vicious;
		Potential potrank;
		Quality quality;
	};
}

