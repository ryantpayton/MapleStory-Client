/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 Daniel Allendorf                                        //
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
#include "Weapon.h"
#include "nlnx\nx.hpp"

namespace Character
{
	Weapon::Weapon(int32_t equipid, const BodyDrawinfo& drawinfo) : Clothing(equipid, drawinfo)
	{
		int32_t prefix = equipid / 10000;
		weptype = static_cast<WpType>(prefix);
		twohanded = (prefix == 138) || (prefix >= 140 && prefix <= 144);

		nl::node infonode = nl::nx::character["Weapon"]["0" + std::to_string(equipid) + ".img"]["info"];
		afterimage = infonode["afterImage"];
		attackspeed = static_cast<uint8_t>(infonode["attackSpeed"]);
		attack = static_cast<uint8_t>(infonode["attack"]);

		/*string sfxstr = infonode["sfx"];
		if (sfxstr == "bow")
		{
			sfx = MSN_WEP_BOW;
		}
		else if (sfxstr == "cBow")
		{
			sfx = MSN_WEP_XBOW;
		}
		else if (sfxstr == "gun")
		{
			sfx = MSN_WEP_GUN;
		}
		else if (sfxstr == "knuckle")
		{
			sfx = MSN_WEP_KNUCKLE;
		}
		else if (sfxstr == "mace")
		{
			sfx = MSN_WEP_MACE;
		}
		else if (sfxstr == "poleArm")
		{
			sfx = MSN_WEP_POLEARM;
		}
		else if (sfxstr == "spear")
		{
			sfx = MSN_WEP_SPEAR;
		}
		else if (sfxstr == "swordL")
		{
			sfx = MSN_WEP_SWORDL;
		}
		else if (sfxstr == "swordS")
		{
			sfx = MSN_WEP_SWORDS;
		}
		else if (sfxstr == "tGlove")
		{
			sfx = MSN_WEP_TGLOVE;
		}
		else
		{
			sfx = MSN_WEP_HANDS;
		}*/
	}

	Weapon::Weapon()
	{
		weptype = WEP_NONE;
	}

	Weapon::WpType Weapon::getweptype() const
	{
		return weptype;
	}

	bool Weapon::istwohanded() const
	{
		return twohanded;
	}

	string Weapon::getafterimage() const
	{
		return afterimage;
	}

	uint8_t Weapon::getspeed() const
	{
		return attackspeed;
	}

	uint8_t Weapon::getattack() const
	{
		return attack;
	}

	uint8_t Weapon::getattackdelay() const
	{
		if (weptype == WEP_NONE)
			return 0;
		else
			return 50 - 25 / attackspeed;
	}

	rectangle2d<int16_t> Weapon::getrange() const
	{
		switch (weptype)
		{
		case WEP_1H_SWORD:
		case WEP_1H_AXE:
		case WEP_1H_MACE:
		case WEP_KNUCKLE:
		case WEP_DAGGER:
			return rectangle2d<int16_t>(-75, -5, -50, 0);
		case WEP_2H_SWORD:
		case WEP_2H_AXE:
		case WEP_2H_MACE:
			return rectangle2d<int16_t>(-100, -5, -50, 0);
		case WEP_SPEAR:
		case WEP_POLEARM:
			return rectangle2d<int16_t>(-125, -5, -50, 0);
		default:
			return rectangle2d<int16_t>();
		}
	}

	Animation Weapon::gethiteffect() const
	{
		return Animation();
	}
}
