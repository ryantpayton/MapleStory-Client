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
		void addequip(const Clothing& cloth);
		void removeequip(Equipslot::Value slot);

		bool isvisible(Equipslot::Value slot) const;
		bool comparelayer(Equipslot::Value slot, Stance::Value stance, Clothing::Layer layer) const;
		bool hasoverall() const;
		bool hasweapon() const;
		bool istwohanded() const;
		CapType getcaptype() const;
		Weapon::Type getweapontype() const;
		std::string getequipname(Equipslot::Value slot) const;

		Optional<const Clothing> getequip(Equipslot::Value slot) const;
		Optional<const Weapon> getweapon() const;

	private:
		const Clothing* equips[Equipslot::LENGTH];
	};
}

