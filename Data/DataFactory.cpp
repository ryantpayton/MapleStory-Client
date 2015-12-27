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
#include "DataFactory.h"
#include <map>

namespace Data
{
	using std::map;

	BodyDrawinfo drawinfo;

	void init()
	{
		drawinfo.init();
	}

	const BodyDrawinfo& getdrawinfo()
	{
		return drawinfo;
	}

	const Bodytype& getbodytype(uint8_t skin)
	{
		static map<uint8_t, Bodytype> bodytypes;
		if (!bodytypes.count(skin))
			bodytypes[skin] = Bodytype(skin, drawinfo);
		return bodytypes[skin];
	}

	const Hairstyle& gethairstyle(int32_t hairid)
	{
		static map<int32_t, Hairstyle> hairstyles;
		if (!hairstyles.count(hairid))
			hairstyles[hairid] = Hairstyle(hairid, drawinfo);
		return hairstyles[hairid];
	}

	const Facetype& getfacetype(int32_t faceid)
	{
		static map<int32_t, Facetype> faces;
		if (!faces.count(faceid))
			faces[faceid] = Facetype(faceid);
		return faces[faceid];
	}

	const ItemData& getitemdata(int32_t itemid)
	{
		int32_t prefix = itemid / 1000000;
		if (prefix == 1)
		{
			return getclothing(itemid);
		}
		else
		{
			static map<int32_t, ItemData> items;
			if (!items.count(itemid))
				items[itemid] = ItemData(itemid);
			return items[itemid];
		}
	}

	const Clothing& getclothing(int32_t itemid)
	{
		int32_t prefix = itemid / 10000;
		if (prefix > 129 && prefix < 200)
		{
			return getweapon(itemid);
		}
		else
		{
			static map<int32_t, Clothing> equips;
			if (!equips.count(itemid))
				equips[itemid] = Clothing(itemid, drawinfo);
			return equips[itemid];
		}
	}

	const Weapon& getweapon(int32_t itemid)
	{
		static map<int32_t, Weapon> weapons;
		if (!weapons.count(itemid))
			weapons[itemid] = Weapon(itemid, drawinfo);
		return weapons[itemid];
	}
}