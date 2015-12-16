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
	using std::uint8_t;
	using std::map;
	using std::string;
	using Graphics::DrawArgument;

	class CharEquips
	{
	public:
		CharEquips();
		~CharEquips();

		void draw(Equipslot, string, CharacterLayer, uint8_t, const DrawArgument&) const;
		void addequip(const Clothing&);
		void removeequip(Equipslot);

		bool isvisible(Equipslot) const;
		bool istwohanded() const;
		Weapon::WpType getweapontype() const;
		const Clothing& getequip(Equipslot) const;
		const Weapon* getweapon() const;

	private:
		map<Equipslot, const Clothing*> equips;
		Clothing nullequip;
	};
}

