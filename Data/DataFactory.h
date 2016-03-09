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
#pragma once
#include "Util\Singleton.h"
#include "Character\Inventory\ItemData.h"
#include "Character\Inventory\BulletData.h"
#include "Character\Look\Body.h"
#include "Character\Look\Hair.h"
#include "Character\Look\Face.h"
#include "Character\Look\Clothing.h"
#include "Character\Look\Weapon.h"
#include "Character\Skill.h"

namespace Data
{
	using std::map;
	using Character::BodyDrawinfo;
	using Character::Body;
	using Character::Hair;
	using Character::Face;
	using Character::ItemData;
	using Character::BulletData;
	using Character::Clothing;
	using Character::Weapon;
	using Character::Skill;

	class DataFactory : public Singleton<DataFactory>
	{
	public:
		void init();

		const BodyDrawinfo& getdrawinfo() const;

		const Body& getbodytype(int32_t skin);
		const Hair& gethairstyle(int32_t hairid);
		const Face& getfacetype(int32_t faceid);
		const ItemData& getitemdata(int32_t itemid);
		const BulletData& getbulletdata(int32_t itemid);
		const Clothing& getclothing(int32_t itemid);
		const Weapon& getweapon(int32_t itemid);
		const Skill& getskill(int32_t skillid);

	private:
		BodyDrawinfo drawinfo;
		map<int32_t, Body> bodytypes;
		map<int32_t, Hair> hairstyles;
		map<int32_t, Face> facetypes;
		map<int32_t, ItemData> itemdata;
		map<int32_t, BulletData> bullets;
		map<int32_t, Clothing> clothing;
		map<int32_t, Weapon> weapons;
		map<int32_t, Skill> skills;
	};
}