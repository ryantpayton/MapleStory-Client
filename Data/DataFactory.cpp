/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 Daniel Allendorf                                               //
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

namespace jrc
{
	void DataFactory::init()
	{
		drawinfo.init();
	}

	const BodyDrawinfo& DataFactory::getdrawinfo() const
	{
		return drawinfo;
	}

	const Body& DataFactory::getbodytype(int32_t skin)
	{
		if (!bodytypes.count(skin))
			bodytypes[skin] = Body(skin, drawinfo);

		return bodytypes[skin];
	}

	const Hair& DataFactory::gethairstyle(int32_t hairid)
	{
		if (!hairstyles.count(hairid))
			hairstyles[hairid] = Hair(hairid, drawinfo);

		return hairstyles[hairid];
	}

	const Face& DataFactory::getfacetype(int32_t faceid)
	{
		if (!facetypes.count(faceid))
			facetypes[faceid] = Face(faceid);

		return facetypes[faceid];
	}

	const ItemData& DataFactory::getitemdata(int32_t itemid)
	{
		int32_t prefix = itemid / 1000000;
		if (prefix == 1)
		{
			return getclothing(itemid);
		}
		else
		{
			int32_t prefix2 = itemid / 10000;
			if (prefix2 == 206)
			{
				return getbulletdata(itemid);
			}
			else
			{
				if (!itemdata.count(itemid))
					itemdata[itemid] = ItemData(itemid);

				return itemdata[itemid];
			}
		}
	}

	const BulletData& DataFactory::getbulletdata(int32_t itemid)
	{
		if (!bullets.count(itemid))
			bullets[itemid] = BulletData(itemid);

		return bullets[itemid];
	}

	const Clothing& DataFactory::getclothing(int32_t itemid)
	{
		int32_t prefix = itemid / 10000;
		if (prefix > 129 && prefix < 200)
		{
			return getweapon(itemid);
		}
		else
		{
			if (!clothing.count(itemid))
				clothing[itemid] = Clothing(itemid, drawinfo);

			return clothing[itemid];
		}
	}

	const Weapon& DataFactory::getweapon(int32_t itemid)
	{
		if (!weapons.count(itemid))
			weapons[itemid] = Weapon(itemid, drawinfo);

		return weapons[itemid];
	}
}