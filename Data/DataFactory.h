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
#include "Character\Inventory\ItemData.h"
#include "Character\Look\Bodytype.h"
#include "Character\Look\Hairstyle.h"
#include "Character\Look\Facetype.h"
#include "Character\Look\Clothing.h"
#include "Character\Look\Weapon.h"
#include "Character\Skill.h"

namespace Data
{
	using Character::BodyDrawinfo;
	using Character::Bodytype;
	using Character::Hairstyle;
	using Character::Facetype;
	using Character::ItemData;
	using Character::Clothing;
	using Character::Weapon;
	using Character::Skill;

	void init();
	const BodyDrawinfo& getdrawinfo();

	const Bodytype& getbodytype(uint8_t skin);
	const Hairstyle& gethairstyle(int32_t hairid);
	const Facetype& getfacetype(int32_t faceid);
	const ItemData& getitemdata(int32_t itemid);
	const Clothing& getclothing(int32_t itemid);
	const Weapon& getweapon(int32_t itemid);
	const Skill& getskill(int32_t skillid);
}