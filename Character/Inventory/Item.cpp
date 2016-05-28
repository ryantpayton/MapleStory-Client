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
#include "Item.h"

namespace jrc
{
	Item::Item(const ItemData& idt, int32_t id, bool cs, int64_t uqi, 
		int64_t exp, int16_t ct, std::string ow, int16_t fl) : idata(idt) {

		iid = id;
		cash = cs;
		uniqueid = uqi;
		expire = exp;
		count = ct;
		owner = ow;
		flag = fl;
	}

	void Item::setcount(int16_t c)
	{
		count = c;
	}

	int16_t Item::getcount() const
	{
		return count;
	}

	int32_t Item::getid() const
	{
		return iid;
	}

	const ItemData& Item::getidata() const
	{
		return idata;
	}
}
