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
			Equipslot::Value value = it.get();
			equips[value] = nullptr;
		}
	}

	void CharEquips::draw(Equipslot::Value slot, Stance::Value stance, Clothing::Layer layer, uint8_t frame, const DrawArgument& args) const 
	{
		const Clothing* cloth = equips[slot];
		if (cloth)
			cloth->draw(stance, layer, frame, args);
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

	bool CharEquips::checkorfalse(Equipslot::Value slot, bool(*check)(const Clothing*)) const
	{
		const Clothing* cloth = equips[slot];
		return (cloth) ? (*check)(cloth) : false;
	}

	bool CharEquips::isvisible(Equipslot::Value slot) const
	{
		return checkorfalse(slot, [](const Clothing* cloth) { 
			return cloth->istransparent(); 
		});
	}

	bool CharEquips::hasoverall() const
	{
		return checkorfalse(Equipslot::TOP, [](const Clothing* cloth){ 
			return cloth->getid() / 10000 == 105;
		});
	}

	bool CharEquips::hasweapon() const
	{
		return getweapon() != nullptr;
	}

	bool CharEquips::istwohanded() const
	{
		const Weapon* weapon = getweapon();
		if (weapon)
			return weapon->istwohanded();
		else
			return false;
	}

	Weapon::Type CharEquips::getweapontype() const
	{
		const Weapon* weapon = getweapon();
		if (weapon)
			return weapon->gettype();
		else
			return Weapon::NONE;
	}

	string CharEquips::getequipname(Equipslot::Value slot) const
	{
		const Clothing* cloth = equips[slot];
		if (cloth == nullptr)
			return "";

		return cloth->getname();
	}

	const Clothing* CharEquips::getequip(Equipslot::Value slot) const
	{
		return equips[slot];
	}

	const Weapon* CharEquips::getweapon() const
	{
		const Clothing* weapon = equips[Equipslot::WEAPON];
		if (weapon == nullptr)
			return nullptr;

		return reinterpret_cast<const Weapon*>(weapon);
	}
}
