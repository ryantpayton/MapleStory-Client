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
#include "UIState.h"

#include <unordered_map>
#include <memory>

namespace jrc
{
	class UIStateLogin : public UIState
	{
	public:
		UIStateLogin();

		void draw(float inter, Point<int16_t> cursor) const override;
		void update() override;

		void doubleclick(Point<int16_t> pos) override;
		void send_key(Keyboard::Keytype type, int32_t action, bool pressed) override;
		Cursor::State send_cursor(Cursor::State mst, Point<int16_t> pos) override;

		void drag_icon(Icon* icon) override;
		void show_equip(UIElement::Type parent, Equip* equip, int16_t slot) override;
		void show_item(UIElement::Type parent, int32_t itemid) override;
		void show_skill(UIElement::Type parent, int32_t skill_id,
			int32_t level, int32_t masterlevel, int64_t expiration) override;
		void clear_tooltip(UIElement::Type parent) override;

		void add(const IElement& element) override;
		void remove(UIElement::Type type) override;
		UIElement* get(UIElement::Type type) const override;
		UIElement* get_front(Point<int16_t> pos) const override;

	private:
		std::unordered_map<UIElement::Type, UIElement::UPtr> elements;
		UIElement::Type focused;
	};
}