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
#include "ItemData.h"

#include "nlnx/nx.hpp"
#include "nlnx/node.hpp"

namespace jrc
{
	ItemData::ItemData(int32_t id)
		: itemid(id) {

		nl::node src;
		nl::node strsrc;

		std::string strprefix = "0" + std::to_string(itemid / 10000);
		std::string strid = "0" + std::to_string(itemid);
		int32_t prefix = itemid / 1000000;
		switch (prefix)
		{
		case 1:
			category = get_eqcategory(itemid);
			src = nl::nx::character[category][strid + ".img"]["info"];
			strsrc = nl::nx::string["Eqp.img"]["Eqp"][category][std::to_string(itemid)];
			break;
		case 2:
			category = "Consume";
			src = nl::nx::item["Consume"][strprefix + ".img"][strid]["info"];
			strsrc = nl::nx::string["Consume.img"][std::to_string(itemid)];
			break;
		case 3:
			category = "Install";
			src = nl::nx::item["Install"][strprefix + ".img"][strid]["info"];
			strsrc = nl::nx::string["Ins.img"][std::to_string(itemid)];
			break;
		case 4:
			category = "Etc";
			src = nl::nx::item["Etc"][strprefix + ".img"][strid]["info"];
			strsrc = nl::nx::string["Etc.img"]["Etc"][std::to_string(itemid)];
			break;
		case 5:
			category = "Cash";
			src = nl::nx::item["Cash"][strprefix + ".img"][strid]["info"];
			strsrc = nl::nx::string["Cash.img"][std::to_string(itemid)];
			break;
		}

		if (src)
		{
			icons[false] = src["icon"];
			icons[true] = src["iconRaw"];
			price = src["price"];

			name = strsrc["name"];
			desc = strsrc["desc"];

			valid = true;
		}
		else
		{
			valid = false;
		}
	}

	std::string ItemData::get_eqcategory(int32_t id) const
	{
		constexpr char* categorynames[15] =
		{
			"Cap", "Accessory", "Accessory", "Accessory", "Coat", "Longcoat",
			"Pants", "Shoes", "Glove", "Shield", "Cape", "Ring", "Accessory",
			"Accessory", "Accessory"
		};

		size_t index = (id / 10000) - 100;
		if (index < 15)
		{
			return categorynames[index];
		}
		else if (index >= 30 && index <= 70)
		{
			return "Weapon";
		}
		else
		{
			return "";
		}
	}

	bool ItemData::is_valid() const
	{
		return valid;
	}

	ItemData::operator bool() const
	{
		return is_valid();
	}

	int32_t ItemData::get_id() const
	{
		return itemid;
	}

	int32_t ItemData::get_price() const
	{
		return price;
	}

	const std::string& ItemData::get_name() const
	{
		return name;
	}

	const std::string& ItemData::get_desc() const
	{
		return desc;
	}

	const std::string& ItemData::get_category() const
	{
		return category;
	}

	const Texture& ItemData::get_icon(bool raw) const
	{
		return icons[raw];
	}
}
