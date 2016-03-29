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
#include "UI.h"
#include "UIStateLogin.h"
#include "UIStateGame.h"
#include "Window.h"

namespace IO
{
	UI::UI()
	{
		state = unique_ptr<UIState>(new UIStateNull());
		enabled = true;
	}

	void UI::init()
	{
		cursor.init();
		changestate(LOGIN);
	}

	void UI::draw(float inter) const
	{
		state->draw(inter, cursor.getposition());

		scrollingnotice.draw(inter);
		cursor.draw(inter);
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

	void UI::changestate(State id)
	{
		switch (id)
		{
		case LOGIN:
			state = unique_ptr<UIState>(new UIStateLogin());
			break;
		case GAME:
			state = unique_ptr<UIState>(new UIStateGame());
			break;
		}
	}

	void UI::sendmouse(Point<int16_t> cursorpos, Cursor::State cursorstate)
	{
		Cursor::State nextstate = state->sendmouse(cursorstate, cursorpos);
		cursor.setstate(nextstate);
		cursor.setposition(cursorpos);
	}

	void UI::sendmouse(bool pressed)
	{
		Cursor::State cursorstate = (pressed && enabled) ? 
			Cursor::CLICKING : 
			Cursor::IDLE;
		Point<int16_t> cursorpos = cursor.getposition();
		sendmouse(cursorpos, cursorstate);

		if (focusedtextfield && pressed)
		{
			Cursor::State tstate = focusedtextfield->sendcursor(cursorpos, pressed);
			switch (tstate)
			{
			case Cursor::IDLE:
				focusedtextfield = Optional<Textfield>();
				break;
			}
		}
	}

	void UI::sendmouse(Point<int16_t> pos)
	{
		sendmouse(pos, cursor.getstate());
	}

	void UI::doubleclick()
	{
		Point<int16_t> pos = cursor.getposition();
		state->doubleclick(pos);
	}

	void UI::sendkey(int32_t keycode, bool pressed)
	{
		if (focusedtextfield)
		{
			bool ctrl = keydown[keyboard.ctrlcode()];
			if (ctrl)
			{
				if (!pressed)
				{
					Keyboard::Action action = keyboard.getctrlaction(keycode);
					switch (action)
					{
					case Keyboard::COPY:
						Window::get().setclipboard(focusedtextfield->gettext());
						break;
					case Keyboard::PASTE:
						focusedtextfield->sendstring(Window::get().getclipboard());
						break;
					}
				}
			}
			else
			{
				bool shift = keydown[keyboard.shiftcode()];
				Keyboard::Mapping mapping = keyboard.gettextmapping(keycode, shift);
				focusedtextfield->sendkey(mapping.type, mapping.action, pressed);
			}
		}
		else
		{
			Optional<const Keyboard::Mapping> mapping = keyboard.getmapping(keycode);
			if (mapping)
			{
				state->sendkey(mapping->type, mapping->action, pressed);
			}
		}

		keydown[keycode] = pressed;
	}

	void UI::setscrollingnotice(string notice)
	{
		scrollingnotice.setnotice(notice);
	}

	void UI::focustextfield(Textfield* tofocus)
	{
		if (focusedtextfield)
		{
			focusedtextfield->setstate(Textfield::NORMAL);
		}

		focusedtextfield = tofocus;
	}

	void UI::dragicon(Icon* icon)
	{
		state->dragicon(icon);
	}

	void UI::addkeymapping(uint8_t no, uint8_t type, int32_t action)
	{
		keyboard.assign(no, type, action);
	}

	void UI::add(const Element& element)
	{
		focusedtextfield = Optional<Textfield>();
		state->add(element);
	}

	void UI::remove(UIElement::Type type)
	{
		focusedtextfield = Optional<Textfield>();
		state->remove(type);
	}

	bool UI::haselement(UIElement::Type type) const
	{
		return getelement(type)
			.ispresent();
	}

	Optional<UIElement> UI::getelement(UIElement::Type type) const
	{
		return state->get(type);
	}
}