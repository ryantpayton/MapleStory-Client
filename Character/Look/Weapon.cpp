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

		using nl::node;
		node infonode = nl::nx::character["Weapon"]["0" + std::to_string(equipid) + ".img"]["info"];

		afterimage = infonode["afterImage"];
		attackspeed = static_cast<uint8_t>(infonode["attackSpeed"]);
		attack = static_cast<uint8_t>(infonode["attack"]);

		string sfxstr = infonode["sfx"];
		if (sfxstr == "bow")
		{
			sfx = SoundFactory::WEP_BOW;
		}
		else if (sfxstr == "cBow")
		{
			sfx = SoundFactory::WEP_XBOW;
		}
		else if (sfxstr == "gun")
		{
			sfx = SoundFactory::WEP_GUN;
		}
		else if (sfxstr == "knuckle")
		{
			sfx = SoundFactory::WEP_KNUCKLE;
		}
		else if (sfxstr == "mace")
		{
			sfx = SoundFactory::WEP_MACE;
		}
		else if (sfxstr == "poleArm")
		{
			sfx = SoundFactory::WEP_POLEARM;
		}
		else if (sfxstr == "spear")
		{
			sfx = SoundFactory::WEP_SPEAR;
		}
		else if (sfxstr == "swordL")
		{
			sfx = SoundFactory::WEP_LONGSWORD;
		}
		else if (sfxstr == "swordS")
		{
			sfx = SoundFactory::WEP_SHORTSWORD;
		}
		else if (sfxstr == "tGlove")
		{
			sfx = SoundFactory::WEP_TGLOVE;
		}
		else
		{
			sfx = SoundFactory::WEP_HANDS;
		}
	}

	Weapon::Weapon()
	{
		weptype = WEP_NONE;
		sfx = SoundFactory::WEP_HANDS;
	}

	void Weapon::playsfx() const
	{
		SoundFactory::play(sfx);
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

	string Weapon::getspeedstring() const
	{
		switch (attackspeed)
		{
		case 1:
			return "FAST (1)";
		case 2:
			return "FAST (2)";
		case 3:
			return "FAST (3)";
		case 4:
			return "FAST (4)";
		case 5:
			return "NORMAL (5)";
		case 6:
			return "NORMAL (6)";
		case 7:
			return "SLOW (7)";
		case 8:
			return "SLOW (8)";
		case 9:
			return "SLOW (9)";
		default:
			return "";
		}
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
