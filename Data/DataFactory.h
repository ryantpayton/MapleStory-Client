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
#include "..\Character\Inventory\ItemData.h"
#include "..\Character\Inventory\BulletData.h"
#include "..\Character\Look\Body.h"
#include "..\Character\Look\Hair.h"
#include "..\Character\Look\Face.h"
#include "..\Character\Look\Clothing.h"
#include "..\Character\Look\Weapon.h"
#include "..\Util\Singleton.h"

namespace jrc
{
	class DataFactory : public Singleton<DataFactory>
	{
	public:
		void init();

		const BodyDrawinfo& getdrawinfo() const;

		const Body& getbodytype(int32_t skin);
		const Hair& gethairstyle(int32_t hairid);
		const Face& getfacetype(int32_t faceid);
		const ItemData& get_itemdata(int32_t itemid);
		const BulletData& get_bulletdata(int32_t itemid);
		const Clothing& get_equip(int32_t itemid);
		const Weapon& get_weapon(int32_t itemid);

	private:
		BodyDrawinfo drawinfo;
		std::map<int32_t, Body> bodytypes;
		std::map<int32_t, Hair> hairstyles;
		std::map<int32_t, Face> facetypes;
		std::map<int32_t, ItemData> itemdata;
		std::map<int32_t, BulletData> bullets;
		std::map<int32_t, Clothing> clothing;
		std::map<int32_t, Weapon> weapons;
	};
}