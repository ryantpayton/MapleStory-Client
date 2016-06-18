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
#include "Cursor.h"
#include "Keyboard.h"
#include "Element.h"
#include "UIState.h"

#include "Components\Icon.h"
#include "Components\Textfield.h"
#include "Components\ScrollingNotice.h"

#include "..\Util\Singleton.h"
#include "..\Util\Optional.h"

#include <unordered_map>

namespace jrc
{
	class UI : public Singleton<UI>
	{
	public:
		enum State
		{
			LOGIN,
			GAME
		};

		UI();

		void init();
		void draw(float alpha) const;
		void update();

		void send_cursor(Point<int16_t> pos);
		void send_cursor(bool pressed);
		void doubleclick();
		void send_key(int32_t keycode, bool pressed);

		void set_scrollnotice(const std::string& notice);
		void focus_textfield(Textfield* textfield);
		void drag_icon(Icon* icon);

		void add_keymapping(uint8_t no, uint8_t type, int32_t action);
		void enable();
		void disable();
		void change_state(State state);

		void add(const IElement& element);
		void remove(UIElement::Type type);

		void clear_tooltip(UIElement::Type type);
		void show_equip(UIElement::Type parent, Equip* equip, int16_t slot);
		void show_item(UIElement::Type parent, int32_t item_id);
		void show_skill(UIElement::Type parent, int32_t skill_id, 
			int32_t level, int32_t masterlevel, int64_t expiration);

		template <class T, typename...Args>
		void add_element(Args&&...args)
		{
			add(Element<T>(args...));
		}

		template <class T>
		Optional<T> get_element();

	private:
		void send_cursor(Point<int16_t> cursorpos, Cursor::State cursorstate);

		UIState::UPtr state;
		Keyboard keyboard;
		Cursor cursor;
		ScrollingNotice scrollingnotice;

		Optional<Textfield> focusedtextfield;
		std::unordered_map<int32_t, bool> is_key_down;

		bool enabled;
	};

	template <class T>
	Optional<T> UI::get_element()
	{
		UIElement::Type type = T::TYPE;
		UIElement* element = state->get(type);
		return static_cast<T*>(element);
	}
}