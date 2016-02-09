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

namespace Character
{
	class CharEquips
	{
	public:
		CharEquips();

		void draw(Equipslot::Value slot, Stance::Value stance, Clothing::Layer layer, uint8_t frame, const DrawArgument& args) const;
		void addequip(const Clothing& cloth);
		void removeequip(Equipslot::Value slot);

		bool isvisible(Equipslot::Value slot) const;
		bool hasoverall() const;
		bool hasweapon() const;
		bool istwohanded() const;
		Weapon::Type getweapontype() const;
		string getequipname(Equipslot::Value slot) const;
		const Clothing* getequip(Equipslot::Value slot) const;
		const Weapon* getweapon() const;

	private:
		bool checkorfalse(Equipslot::Value slot, bool(*check)(const Clothing*)) const;

		const Clothing* equips[Equipslot::LENGTH];
	};
}

