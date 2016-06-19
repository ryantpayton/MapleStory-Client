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
#include "CharEquips.h"

namespace jrc
{
	CharEquips::CharEquips()
	{
		for (Equipslot::Value slot : Equipslot::values.keys())
		{
			equips[slot] = nullptr;
		}
	}

	void CharEquips::draw(Equipslot::Value slot, Stance::Value stance, Clothing::Layer layer, uint8_t frame, const DrawArgument& args) const 
	{
		Optional<const Clothing> equip = get_equip(slot);
		if (equip)
		{
			equip->draw(stance, layer, frame, args);
		}
	}

	void CharEquips::add_equip(const Clothing& eq)
	{
		Equipslot::Value slot = eq.geteqslot();
		equips[slot] = &eq;
	}

	void CharEquips::remove_equip(Equipslot::Value slot)
	{
		equips[slot] = nullptr;
	}

	bool CharEquips::is_visible(Equipslot::Value slot) const
	{
		return !get_equip(slot)
			.maportrue(&Clothing::istransparent);
	}

	bool CharEquips::comparelayer(Equipslot::Value slot, Stance::Value stance, Clothing::Layer layer) const
	{
		return get_equip(slot)
			.maporfalse(&Clothing::contains_layer, stance, layer);
	}

	bool CharEquips::has_overall() const
	{
		return get_equip(Equipslot::TOP)
			.mapordefault(&Clothing::get_id, 0) / 10000 == 105;
	}

	bool CharEquips::has_weapon() const
	{
		return get_weapon()
			.ispresent();
	}

	bool CharEquips::is_twohanded() const
	{
		return get_weapon()
			.maporfalse(&Weapon::is_twohanded);
	}

	CharEquips::CapType CharEquips::getcaptype() const
	{
		Optional<const Clothing> cap = get_equip(Equipslot::CAP);
		if (cap)
		{
			std::string vslot = cap->getvslot();
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

	uint8_t CharEquips::get_attackspeed() const
	{
		return get_weapon()
			.mapordefault(&Weapon::getspeed, uint8_t(0));
	}

	Weapon::Type CharEquips::get_weapontype() const
	{
		return get_weapon()
			.mapordefault(&Weapon::gettype, Weapon::NONE);
	}

	std::string CharEquips::get_equipname(Equipslot::Value slot) const
	{
		return get_equip(slot)
			.mapordefault(&Clothing::get_name, std::string(""));
	}

	Optional<const Clothing> CharEquips::get_equip(Equipslot::Value slot) const
	{
		return equips[slot];
	}

	Optional<const Weapon> CharEquips::get_weapon() const
	{
		return get_equip(Equipslot::WEAPON)
			.reinterpret<const Weapon>();
	}
}
