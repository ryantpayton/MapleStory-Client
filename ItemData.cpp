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
#include "nx.hpp"

namespace Data
{
	ItemData::ItemData(int id)
	{
		node src;
		node strsrc;

		int prefix = id / 1000000;
		switch (prefix)
		{
		case 1:
			geteqcategory(id);
			src = nx::character[category]["0" + to_string(id) + ".img"];
			strsrc = nx::string["Eqp.img"]["Eqp"][category][to_string(id)];
			break;
		case 2:
			category = "Consume";
			src = nx::item["Consume"]["0" + to_string(id / 10000) + ".img"]["0" + to_string(id)]["info"];
			strsrc = nx::string["Consume.img"][to_string(id)];
			break;
		case 3:
			category = "Install";
			src = nx::item["Install"]["0" + to_string(id / 10000) + ".img"]["0" + to_string(id)]["info"];
			strsrc = nx::string["Ins.img"][to_string(id)];
			break;
		case 4:
			category = "Etc";
			src = nx::item["Etc"]["0" + to_string(id / 10000) + ".img"]["0" + to_string(id)]["info"];
			strsrc = nx::string["Etc.img"]["Etc"][to_string(id)];
			break;
		case 5:
			category = "Cash";
			src = nx::item["Cash"]["0" + to_string(id / 10000) + ".img"]["0" + to_string(id)]["info"];
			strsrc = nx::string["Cash.img"][to_string(id)];
			break;
		}

		if (src.size() > 0)
		{
			icons.add(false, new Texture(src["icon"]));
			icons.add(true, new Texture(src["iconRaw"]));

			name = strsrc["name"];
			desc = strsrc["desc"];

			/*txtargs descargs;
			descargs.text = desc;
			descargs.color = TXC_WHITE;
			desctext = itemtext(descargs, DWF_12LL, vector2d(150, 0));*/
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
		name = "";
		desc = "";
	}

	void ItemData::geteqcategory(int id)
	{
		int prefix = id / 10000;
		switch (prefix)
		{
		case 100:
			category = "Cap";
			break;
		case 104:
			category = "Coat";
			break;
		case 105:
			category = "Longcoat";
			break;
		case 106:
			category = "Pants";
			break;
		case 107:
			category = "Shoes";
			break;
		case 108:
			category = "Glove";
			break;
		case 109:
			category = "Shield";
			break;
		case 110:
			category = "Cape";
			break;
		case 111:
			category = "Ring";
			break;
		case 101:
		case 102:
		case 103:
		case 112:
		case 113:
		case 114:
			category = "Accessory";
			break;
		default:
			if (prefix >= 130 && prefix <= 170)
			{
				category = "Weapon";
			}
			else
			{
				category = "";
			}
		}
	}

	bool ItemData::isloaded() const
	{
		return loaded;
	}
}
