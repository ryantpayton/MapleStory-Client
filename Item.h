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
#include "ItemData.h"
#include "InPacket.h"

using namespace Data;

namespace Character
{
	class Item
	{
	public:
		Item(const ItemData&, int, bool, int64_t, int64_t, short, string, short);
		~Item(){}
		int getid();
	protected:
		const ItemData& idata;
		int iid;
		bool cash;
		int64_t uniqueid;
		int64_t expire;
		short count;
		string owner;
		short flag;
	};
}

