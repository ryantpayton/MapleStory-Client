/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
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
#include "IO\Components\Icon.h"
#include "IO\Components\EquipTooltip.h"
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

		UIEquipInventory();

		void draw(float inter) const override;
		void buttonpressed(uint16_t buttonid) override;
		void togglehide() override;
		void doubleclick(vector2d<int16_t> position) override;
		void icondropped(int16_t identifier) override;
		void dropicon(vector2d<int16_t> position, Type type, int16_t identifier) override;
		Cursor::State sendmouse(bool pressed, vector2d<int16_t> position) override;

		void modify(int16_t pos, int8_t mode, int16_t arg);

	private:
		UIEquipInventory& operator = (const UIEquipInventory&) = delete;

		void loadicons();
		void addicon(int16_t slot);
		int16_t slotbypos(vector2d<int16_t> position) const;

		const Inventory& inventory;

		map<int16_t, vector2d<int16_t>> iconpositions;
		EquipTooltip tooltip;
		vector2d<int16_t> cursorposition;
		map<int16_t, Icon> icons;

		vector<Texture> pettextures;
		bool showpetequips;
	};

	class ElementEquipInventory : public Element
	{
		bool isunique() const override
		{
			return true;
		}

		UIElement::Type type() const override
		{
			return UIElement::EQUIPINVENTORY;
		}

		UIElement* instantiate() const override
		{
			return new UIEquipInventory();
		}
	};
}