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
#pragma once
#include "Clothing.h"
#include "Weapon.h"

#include "..\..\Util\Optional.h"

namespace jrc
{
	class CharEquips
	{
	public:
		enum CapType
		{
			   NONE,
			   HEADBAND,
			   HAIRPIN,
			   HALFCOVER,
			   FULLCOVER
		};

		CharEquips();

		void draw(Equipslot::Value slot, Stance::Value stance, Clothing::Layer layer, uint8_t frame, const DrawArgument& args) const;
		void add_equip(const Clothing& cloth);
		void remove_equip(Equipslot::Value slot);

		bool is_visible(Equipslot::Value slot) const;
		bool comparelayer(Equipslot::Value slot, Stance::Value stance, Clothing::Layer layer) const;
		bool has_overall() const;
		bool has_weapon() const;
		bool is_twohanded() const;
		CapType getcaptype() const;
		uint8_t get_attackspeed() const;
		Weapon::Type get_weapontype() const;
		std::string get_equipname(Equipslot::Value slot) const;

		Optional<const Clothing> get_equip(Equipslot::Value slot) const;
		Optional<const Weapon> get_weapon() const;

	private:
		const Clothing* equips[Equipslot::LENGTH];
	};
}

