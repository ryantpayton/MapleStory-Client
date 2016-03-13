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
#include "CharEquips.h"

namespace Character
{
	CharEquips::CharEquips()
	{
		for (auto it = Equipslot::getit(); it.hasnext(); it.increment())
		{
			Equipslot::Value slot = it.get();
			equips[slot] = nullptr;
		}
	}

	void CharEquips::draw(Equipslot::Value slot, Stance::Value stance, Clothing::Layer layer, uint8_t frame, const DrawArgument& args) const 
	{
		Optional<const Clothing> equip = getequip(slot);
		if (equip)
		{
			equip->draw(stance, layer, frame, args);
		}
	}

	void CharEquips::addequip(const Clothing& eq)
	{
		Equipslot::Value slot = eq.geteqslot();
		equips[slot] = &eq;
	}

	void CharEquips::removeequip(Equipslot::Value slot)
	{
		equips[slot] = nullptr;
	}

	bool CharEquips::isvisible(Equipslot::Value slot) const
	{
		return !getequip(slot)
			.maportrue(&Clothing::istransparent);
	}

	bool CharEquips::comparelayer(Equipslot::Value slot, Stance::Value stance, Clothing::Layer layer) const
	{
		return getequip(slot)
			.maporfalse(&Clothing::islayer, stance, layer);
	}

	bool CharEquips::hasoverall() const
	{
		return getequip(Equipslot::TOP)
			.mapordefault(&Clothing::getid, 0) / 10000 == 105;
	}

	bool CharEquips::hasweapon() const
	{
		return getweapon()
			.ispresent();
	}

	bool CharEquips::istwohanded() const
	{
		return getweapon()
			.maporfalse(&Weapon::istwohanded);
	}

	CharEquips::CapType CharEquips::getcaptype() const
	{
		Optional<const Clothing> cap = getequip(Equipslot::CAP);
		if (cap)
		{
			string vslot = cap->getvslot();
			if (vslot == "CpH1H5")
				return HALFCOVER;
			else if (vslot == "CpH1H5AyAs")
				return FULLCOVER;
			else if (vslot == "CpH5")
				return HEADBAND;
			else
				return NONE;
		}
		else
		{
			return NONE;
		}
	}

	Weapon::Type CharEquips::getweapontype() const
	{
		return getweapon()
			.mapordefault(&Weapon::gettype, Weapon::NONE);
	}

	string CharEquips::getequipname(Equipslot::Value slot) const
	{
		return getequip(slot)
			.mapordefault(&Clothing::getname, string(""));
	}

	Optional<const Clothing> CharEquips::getequip(Equipslot::Value slot) const
	{
		return equips[slot];
	}

	Optional<const Weapon> CharEquips::getweapon() const
	{
		return getequip(Equipslot::WEAPON)
			.reinterpret<const Weapon>();
	}
}
