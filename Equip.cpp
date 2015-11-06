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
#include "Equip.h"

namespace Character
{
	Equip::Equip(const ItemData& eqd, int32_t id, bool cs, int64_t uqi, int64_t exp, uint8_t sl, uint8_t lv, map<Equipstat, int16_t> st, string ow, int16_t fl, uint8_t ilv, int16_t iexp, int32_t vic) : Item(eqd, id, cs, uqi, exp, 1, ow, fl)
	{
		slots = sl;
		level = lv;
		stats = st;
		itemlevel = ilv;
		itemexp = iexp;
		vicious = vic;
	}
}
