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
#include "IO\Components\Icon.h"
#include "IO\Components\EquipTooltip.h"
#include "IO\Components\ItemTooltip.h"
#include "IO\Components\Slider.h"
#include "Character\Inventory\Inventory.h"

namespace IO
{
	using std::pair;
	using Graphics::Animation;
	using Graphics::Text;
	using Character::Inventory;

	// The Item inventory.
	class UIItemInventory : public UIDragElement
	{
	public:
		enum Buttons
		{
			BT_TAB_EQUIP,
			BT_TAB_USE,
			BT_TAB_ETC,
			BT_TAB_SETUP,
			BT_TAB_CASH,
			BT_DROPMESO,
			BT_POINTS,
			BT_GATHER,
			BT_SORT,
			BT_EXPAND,
			BT_ITEMPOT,
			BT_UPGRADE,
			BT_MAGNIFY,
			BT_BITCASE
		};

		UIItemInventory();

		void draw(float inter) const override;
		void update() override;
		void buttonpressed(uint16_t buttonid) override;
		void doubleclick(vector2d<int16_t> position) override;
		void icondropped(int16_t identifier) override;
		void dropicon(vector2d<int16_t> position, Type type, int16_t identifier) override;
		void togglehide() override;
		Cursor::State sendmouse(bool pressed, vector2d<int16_t> position) override;

		void modify(Inventory::Type type, int16_t pos, int8_t mode, int16_t arg);
		void enablesort();
		void enablegather();

	private:
		UIItemInventory& operator = (const UIItemInventory&) = delete;

		void loadicons();
		void updateslot(int16_t slot);

		string getmesostr() const;
		int16_t slotbypos(vector2d<int16_t> position) const;
		vector2d<int16_t> getslotpos(int16_t slot) const;
		vector2d<int16_t> gettabpos(Inventory::Type tab) const;
		uint16_t buttonbytab(Inventory::Type tab) const;

		const Inventory& inventory;

		Animation newitemslot;
		Animation newitemtab;
		Texture projectile;
		Text mesolabel;
		Slider slider;

		EquipTooltip eqtooltip;
		ItemTooltip ittooltip;
		vector2d<int16_t> cursorposition;
		map<int16_t, Icon> icons;

		Inventory::Type tab;
		pair<int16_t, int16_t> slotrange;
		Inventory::Type newtab;
		int16_t newslot;
	};

	class ElementItemInventory : public Element
	{
		bool isunique() const override
		{
			return true;
		}

		UIElement::Type type() const override
		{
			return UIElement::ITEMINVENTORY;
		}

		UIElement* instantiate() const override
		{
			return new UIItemInventory();
		}
	};
}