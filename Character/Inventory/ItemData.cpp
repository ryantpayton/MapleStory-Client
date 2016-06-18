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

#include "nlnx\nx.hpp"

namespace jrc
{
	ItemData::ItemData(int32_t id)
	{
		nl::node src;
		nl::node strsrc;

		int32_t prefix = id / 1000000;
		switch (prefix)
		{
		case 1:
			category = geteqcategory(id);
			src = nl::nx::character[category]["0" + std::to_string(id) + ".img"]["info"];
			strsrc = nl::nx::string["Eqp.img"]["Eqp"][category][std::to_string(id)];
			break;
		case 2:
			category = "Consume";
			src = nl::nx::item["Consume"]["0" + std::to_string(id / 10000) + ".img"]["0" + std::to_string(id)]["info"];
			strsrc = nl::nx::string["Consume.img"][std::to_string(id)];
			break;
		case 3:
			category = "Install";
			src = nl::nx::item["Install"]["0" + std::to_string(id / 10000) + ".img"]["0" + std::to_string(id)]["info"];
			strsrc = nl::nx::string["Ins.img"][std::to_string(id)];
			break;
		case 4:
			category = "Etc";
			src = nl::nx::item["Etc"]["0" + std::to_string(id / 10000) + ".img"]["0" + std::to_string(id)]["info"];
			strsrc = nl::nx::string["Etc.img"]["Etc"][std::to_string(id)];
			break;
		case 5:
			category = "Cash";
			src = nl::nx::item["Cash"]["0" + std::to_string(id / 10000) + ".img"]["0" + std::to_string(id)]["info"];
			strsrc = nl::nx::string["Cash.img"][std::to_string(id)];
			break;
		}

		if (src.size() > 0)
		{
			icons[false] = src["icon"];
			icons[true] = src["iconRaw"];
			price = src["price"];

			name = strsrc["name"];
			desc = strsrc["desc"];

			loaded = true;
		}
		else
		{
			loaded = false;
		}

		itemid = id;
	}

	ItemData::ItemData()
	{
		loaded = false;
		itemid = 0;
		price = 0;
		name = "";
		desc = "";
	}

	ItemData::~ItemData() {}

	std::string ItemData::geteqcategory(int32_t id) const
	{
		static constexpr char* categorynames[15] =
		{
			"Cap", "Accessory", "Accessory", "Accessory", "Coat", "Longcoat",
			"Pants", "Shoes", "Glove", "Shield", "Cape", "Ring", "Accessory",
			"Accessory", "Accessory"
		};

		int32_t prefix = (id / 10000) - 100;
		if (prefix >= 0 && prefix < 15)
			return categorynames[prefix];
		else if (prefix >= 30 && prefix <= 70)
			return "Weapon";
		else
			return "";
	}

	std::string ItemData::getcategory() const
	{
		return category;
	}

	bool ItemData::is_loaded() const
	{
		return loaded;
	}

	int32_t ItemData::get_id() const
	{
		return itemid;
	}

	int32_t ItemData::getprice() const
	{
		return price;
	}

	std::string ItemData::get_name() const
	{
		return name;
	}

	std::string ItemData::getdesc() const
	{
		return desc;
	}

	const Texture& ItemData::geticon(bool raw) const
	{
		return icons[raw];
	}
}
