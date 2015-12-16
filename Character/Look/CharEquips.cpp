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
	CharEquips::CharEquips() {}

	CharEquips::~CharEquips() {}

	void CharEquips::draw(Equipslot slot, string stance, CharacterLayer layer, 
		uint8_t frame, const DrawArgument& args) const {

		if (equips.count(slot))
		{
			equips.at(slot)->draw(stance, layer, frame, args);
		}
	}

	void CharEquips::addequip(const Clothing& eq)
	{
		Equipslot slot = eq.geteqslot();
		equips[slot] = &eq;
	}

	void CharEquips::removeequip(Equipslot slot)
	{
		equips.erase(slot);
	}

	bool CharEquips::isvisible(Equipslot slot) const
	{
		return !getequip(slot).istransparent();
	}

	bool CharEquips::istwohanded() const
	{
		const Weapon* weapon = getweapon();
		if (weapon)
			return weapon->istwohanded();
		else
			return false;
	}

	Weapon::WpType CharEquips::getweapontype() const
	{
		const Weapon* weapon = getweapon();
		if (weapon)
			return weapon->getweptype();
		else
			return Weapon::WEP_NONE;
	}

	const Clothing& CharEquips::getequip(Equipslot slot) const
	{
		if (equips.count(slot))
			return *equips.at(slot);
		else
			return nullequip;
	}

	const Weapon* CharEquips::getweapon() const
	{
		const Clothing& weapon = getequip(EQL_WEAPON);
		if (weapon.geteqslot() == EQL_WEAPON)
			return static_cast<const Weapon*>(&weapon);
		else
			return nullptr;
	}
}
