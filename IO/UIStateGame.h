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
#include "UIState.h"

#include "Components\EquipTooltip.h"
#include "Components\ItemTooltip.h"

#include "..\Util\Optional.h"

#include <unordered_map>
#include <list>
#include <memory>

namespace jrc
{
	class UIStateGame : public UIState
	{
	public:
		UIStateGame();
		
		void draw(float inter, Point<int16_t> cursor) const override;
		void update() override;

		void doubleclick(Point<int16_t> pos) override;
		void sendkey(Keyboard::Keytype type, int32_t action, bool pressed) override;
		Cursor::State sendmouse(Cursor::State mst, Point<int16_t> pos) override;

		void dragicon(Icon* icon) override;
		void showequip(UIElement::Type parent, Equip* equip, int16_t slot) override;
		void showitem(UIElement::Type parent, int32_t itemid) override;
		void cleartooltip(UIElement::Type parent) override;

		void add(const Element& element) override;
		void remove(UIElement::Type type) override;
		UIElement* get(UIElement::Type type) const override;
		UIElement* getfront(Point<int16_t> pos) const override;

	private:
		void dropicon(const Icon& icon, Point<int16_t> pos);

		std::unordered_map<UIElement::Type, UIElement::UPtr> elements;
		std::list<UIElement::Type> elementorder;
		UIElement::Type focused;

		EquipTooltip eqtooltip;
		ItemTooltip ittooltip;
		Optional<Tooltip> tooltip;
		UIElement::Type tooltipparent;

		Optional<Icon> draggedicon;
	};
}