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
#include "IO\Element.h"
#include "IO\UIDragElement.h"
#include "Character\Inventory\Inventory.h"

namespace IO
{
	using std::map;
	using Character::Inventory;
	using Graphics::Texture;

	// The Equip inventory.
	class UIEquipInventory : public UIDragElement
	{
	public:
		enum Buttons
		{
			BT_TOGGLEPETS
		};

		UIEquipInventory(const Inventory& inventory);
		~UIEquipInventory();

		void draw(float inter) const override;
		void buttonpressed(uint16_t buttonid) override;

	private:
		UIEquipInventory& operator = (const UIEquipInventory&) = delete;
		void loadicons();

		const Inventory& inventory;
		map<int16_t, vector2d<int16_t>> iconpositions;

		vector<Texture> pettextures;
		bool showpetequips;
	};

	class ElementEquipInventory : public Element
	{
	public:
		ElementEquipInventory(const Inventory& inv) : inventory(inv) {}

		bool isunique() const override
		{
			return true;
		}

		UIType type() const override
		{
			return EQUIPINVENTORY;
		}

		UIElement* instantiate() const override
		{
			return new UIEquipInventory(inventory);
		}

	private:
		ElementEquipInventory& operator = (const ElementEquipInventory&) = delete;

		const Inventory& inventory;
	};
}