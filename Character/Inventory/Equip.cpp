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
#include "Equip.h"

namespace jrc
{
	Equip::Equip(const ItemData& eqd, int32_t id, bool cs, int64_t uqi, int64_t exp, 
		uint8_t sl, uint8_t lv, const EnumMap<Equipstat::Value, uint16_t>& st, const std::string& ow, 
		int16_t fl, uint8_t ilv, int16_t iexp, int32_t vic) : Item(eqd, id, cs, uqi, exp, 1, ow, fl) {

		slots = sl;
		level = lv;
		stats = st;
		itemlevel = ilv;
		itemexp = iexp;
		vicious = vic;

		potrank = POT_NONE;
		checkquality();
	}

	void Equip::checkquality()
	{
		int16_t totaldelta = 0;
		const Clothing& cloth = getcloth();
		for (auto iter : stats)
		{
			Equipstat::Value es = iter.first;
			uint16_t stat = iter.second;
			totaldelta += stat - cloth.getdefstat(es);
		}

		if (totaldelta < -4)
			quality = EQQ_GREY;
		else if (totaldelta < 7)
			quality = (level > 0) ? (totaldelta > 0) ? EQQ_ORANGE : EQQ_GREY : EQQ_WHITE;
		else if (totaldelta < 14)
			quality = EQQ_BLUE;
		else if (totaldelta < 21)
			quality = EQQ_VIOLET;
		else
			quality = EQQ_GOLD;
	}

	uint8_t Equip::get_slots() const
	{
		return slots;
	}

	uint8_t Equip::get_level() const
	{
		return level;
	}

	uint8_t Equip::getitemlevel() const
	{
		return itemlevel;
	}

	uint16_t Equip::get_stat(Equipstat::Value type) const
	{
		return stats[type];
	}

	int32_t Equip::getvicious() const
	{
		return vicious;
	}

	Equip::Potential Equip::getpotrank() const
	{
		return potrank;
	}

	Equip::Quality Equip::getquality() const
	{
		return quality;
	}

	const Clothing& Equip::getcloth() const
	{
		return reinterpret_cast<const Clothing&>(idata);
	}
}
