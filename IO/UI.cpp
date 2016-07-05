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
#include "UI.h"

#include "UIStateLogin.h"
#include "UIStateGame.h"
#include "Window.h"

namespace jrc
{
	UI::UI()
	{
		state = std::make_unique<UIStateNull>();
		enabled = true;
	}

	void UI::init()
	{
		cursor.init();
		change_state(LOGIN);
	}

	void UI::draw(float alpha) const
	{
		state->draw(alpha, cursor.get_position());

		scrollingnotice.draw(alpha);
		cursor.draw(alpha);
	}

	void UI::update()
	{
		state->update();

		scrollingnotice.update();
		cursor.update();
	}

	void UI::enable()
	{
		enabled = true;
	}

	void UI::disable()
	{
		enabled = false;
	}

	void UI::change_state(State id)
	{
		switch (id)
		{
		case LOGIN:
			state = std::make_unique<UIStateLogin>();
			break;
		case GAME:
			state = std::make_unique<UIStateGame>();
			break;
		}
	}

	void UI::quit()
	{
		quitted = true;
	}

	bool UI::not_quitted() const
	{
		return !quitted;
	}

	void UI::send_cursor(Point<int16_t> cursorpos, Cursor::State cursorstate)
	{
		Cursor::State nextstate = state->send_cursor(cursorstate, cursorpos);
		cursor.set_state(nextstate);
		cursor.set_position(cursorpos);
	}

	void UI::send_cursor(bool pressed)
	{
		Cursor::State cursorstate = (pressed && enabled) ? 
			Cursor::CLICKING : 
			Cursor::IDLE;
		Point<int16_t> cursorpos = cursor.get_position();
		send_cursor(cursorpos, cursorstate);

		if (focusedtextfield && pressed)
		{
			Cursor::State tstate = focusedtextfield->send_cursor(cursorpos, pressed);
			switch (tstate)
			{
			case Cursor::IDLE:
				focusedtextfield = {};
				break;
			}
		}
	}

	void UI::send_cursor(Point<int16_t> pos)
	{
		send_cursor(pos, cursor.get_state());
	}

	void UI::doubleclick()
	{
		Point<int16_t> pos = cursor.get_position();
		state->doubleclick(pos);
	}

	void UI::send_key(int32_t keycode, bool pressed)
	{
		if (focusedtextfield)
		{
			bool ctrl = is_key_down[keyboard.ctrlcode()];
			if (ctrl)
			{
				if (!pressed)
				{
					KeyAction::Id action = keyboard.get_ctrl_action(keycode);
					switch (action)
					{
					case KeyAction::COPY:
						Window::get().setclipboard(focusedtextfield->get_text());
						break;
					case KeyAction::PASTE:
						focusedtextfield->add_string(Window::get().getclipboard());
						break;
					}
				}
			}
			else
			{
				bool shift = is_key_down[keyboard.shiftcode()];
				Keyboard::Mapping mapping = keyboard.get_text_mapping(keycode, shift);
				focusedtextfield->send_key(mapping.type, mapping.action, pressed);
			}
		}
		else
		{
			Keyboard::Mapping mapping = keyboard.get_mapping(keycode);
			if (mapping.type)
			{
				state->send_key(mapping.type, mapping.action, pressed);
			}
		}

		is_key_down[keycode] = pressed;
	}

	void UI::send_menu(KeyAction::Id action)
	{
		state->send_key(KeyType::MENU, action, true);
	}

	void UI::set_scrollnotice(const std::string& notice)
	{
		scrollingnotice.setnotice(notice);
	}

	void UI::focus_textfield(Textfield* tofocus)
	{
		if (focusedtextfield)
		{
			focusedtextfield->set_state(Textfield::NORMAL);
		}

		focusedtextfield = tofocus;
	}

	void UI::drag_icon(Icon* icon)
	{
		state->drag_icon(icon);
	}

	void UI::add_keymapping(uint8_t no, uint8_t type, int32_t action)
	{
		keyboard.assign(no, type, action);
	}

	void UI::clear_tooltip(Tooltip::Parent parent)
	{
		state->clear_tooltip(parent);
	}

	void UI::show_equip(Tooltip::Parent parent, int16_t slot)
	{
		state->show_equip(parent, slot);
	}

	void UI::show_item(Tooltip::Parent parent, int32_t item_id)
	{
		state->show_item(parent, item_id);
	}

	void UI::show_skill(Tooltip::Parent parent, int32_t skill_id, 
		int32_t level, int32_t masterlevel, int64_t expiration) {

		state->show_skill(parent, skill_id, level, masterlevel, expiration);
	}

	void UI::remove(UIElement::Type type)
	{
		focusedtextfield = {};
		state->remove(type);
	}
}