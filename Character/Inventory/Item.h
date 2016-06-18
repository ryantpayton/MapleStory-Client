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
#include "ItemData.h"

namespace jrc
{
	class Item
	{
	public:
		Item(const ItemData&, int32_t, bool, int64_t, int64_t, int16_t, std::string, int16_t);

		void setcount(int16_t count);

		int16_t getcount() const;
		int32_t get_id() const;

		const ItemData& getidata() const;

	protected:
		const ItemData& idata;
		int32_t iid;
		bool cash;
		int64_t uniqueid;
		int64_t expire;
		int16_t count;
		std::string owner;
		int16_t flag;

	private:
		Item(const Item&) = delete;
		Item& operator = (const Item&) = delete;
	};
}

