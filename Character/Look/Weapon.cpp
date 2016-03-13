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
		type = typebyvalue(prefix);
		twohanded = (prefix == STAFF) || (prefix >= SWORD_2H && prefix <= POLEARM) || (prefix == CROSSBOW);

		using nl::node;
		node src = nl::nx::character["Weapon"]["0" + std::to_string(equipid) + ".img"]["info"];

		int32_t standno = src["stand"];
		switch (standno)
		{
		case 1:
			stand = Stance::STAND1;
			break;
		case 2:
			stand = Stance::STAND2;
			break;
		default:
			stand = twohanded ? Stance::STAND2 : Stance::STAND1;
		}
		int32_t walkno = src["walk"];
		switch (walkno)
		{
		case 1:
			walk = Stance::WALK1;
			break;
		case 2:
			walk = Stance::WALK2;
			break;
		default:
			walk = twohanded ? Stance::WALK2 : Stance::WALK1;
		}

		afterimage = src["afterImage"];
		attackspeed = static_cast<uint8_t>(src["attackSpeed"]);
		attack = static_cast<uint8_t>(src["attack"]);

		node soundsrc = nl::nx::sound["Weapon.img"][src["sfx"]];
		firstattack = soundsrc["Attack"];
		secondattack = soundsrc["Attack2"];
	}

	Weapon::Weapon()
	{
		type = NONE;
	}

	Weapon::Type Weapon::gettype() const
	{
		return type;
	}

	Stance::Value Weapon::getstand() const
	{
		return stand;
	}

	Stance::Value Weapon::getwalk() const
	{
		return walk;
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
		if (type == NONE)
			return 0;
		else
			return 50 - 25 / attackspeed;
	}

	rectangle2d<int16_t> Weapon::getrange() const
	{
		switch (type)
		{
		case SWORD_1H:
		case AXE_1H:
		case MACE_1H:
		case KNUCKLE:
		case DAGGER:
		case BOW:
		case CROSSBOW:
		case GUN:
		case CLAW:
			return rectangle2d<int16_t>(-75, -5, -50, 0);
		case SWORD_2H:
		case AXE_2H:
		case MACE_2H:
			return rectangle2d<int16_t>(-100, -5, -50, 0);
		case SPEAR:
		case POLEARM:
			return rectangle2d<int16_t>(-125, -5, -50, 0);
		default:
			return rectangle2d<int16_t>();
		}
	}

	Animation Weapon::gethiteffect() const
	{
		return Animation();
	}

	const Sound& Weapon::getsound(bool degenerate) const
	{
		return degenerate ? secondattack : firstattack;
	}
}
