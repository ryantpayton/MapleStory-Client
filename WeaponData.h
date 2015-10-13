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
#include "EquipData.h"

namespace Data
{
	enum Weapontype
	{
		WEP_NONE = 0,
		WEP_1H_SWORD = 130,
		WEP_1H_AXE = 131,
		WEP_1H_MACE = 132,
		WEP_DAGGER = 133,
		WEP_WAND = 137,
		WEP_STAFF = 138,
		WEP_2H_SWORD = 140,
		WEP_2H_AXE = 141,
		WEP_2H_MACE = 142,
		WEP_SPEAR = 143,
		WEP_POLEARM = 144,
		WEP_BOW = 145,
		WEP_CROSSBOW = 146,
		WEP_CLAW = 147,
		WEP_KNUCKLE = 148,
		WEP_GUN = 149,
		WEP_CASH = 170
	};

	class WeaponData : public EquipData
	{
	public:
		WeaponData(int, BodyDrawinfo*);
		Weapontype getweptype() { return weptype; }
		bool istwohanded() { return twohanded; }
		string getafterimage() { return afterimage; }
		uint8_t getspeed() { return attackspeed; }
		uint8_t getattack() { return attack; }
	private:
		Weapontype weptype;
		bool twohanded;
		string afterimage;
		//maplesound sfx;
		uint8_t attackspeed;
		uint8_t attack;
	};
}

