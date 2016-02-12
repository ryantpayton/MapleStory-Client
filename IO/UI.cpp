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

namespace IO
{
	UI::UI()
	{
		state = unique_ptr<UIState>(new UIStateNull());
		focusedtextfield = nullptr;
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
		cursor.draw(inter);
	}

	void UI::update()
	{
		state->update();
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

	void UI::sendmouse(bool pressed, vector2d<int16_t> pos)
	{
		Cursor::State cursorstate = (pressed && enabled) ? Cursor::CLICKING : Cursor::IDLE;
		cursorstate = state->sendmouse(cursorstate, pos);

		cursor.setposition(pos);
		cursor.setstate(cursorstate);
	}

	void UI::sendmouse(vector2d<int16_t> pos)
	{
		Cursor::State cursorstate = state->sendmouse(cursor.getstate(), pos);

		cursor.setposition(pos);
		cursor.setstate(cursorstate);
	}

	void UI::doubleclick(vector2d<int16_t> pos)
	{
		state->doubleclick(pos);
	}

	void UI::sendkey(int32_t keycode, bool pressed)
	{
		if (focusedtextfield)
		{
			bool shift = keydown[keyboard.shiftcode()];
			Keyboard::Keymapping mapping = keyboard.gettextmapping(keycode, shift);
			focusedtextfield->sendkey(mapping.type, mapping.action, pressed);
		}
		else
		{
			const Keyboard::Keymapping* mapping = keyboard.getmapping(keycode);
			if (mapping)
			{
				state->sendkey(mapping->type, mapping->action, pressed);
			}
		}

		keydown[keycode] = pressed;
	}

	void UI::focustextfield(Textfield* tofocus)
	{
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
		state->add(element);
	}

	void UI::remove(Element::UIType type)
	{
		focusedtextfield = nullptr;
		state->remove(type);
	}

	bool UI::haselement(Element::UIType type) const
	{
		return getelement(type) != nullptr;
	}

	UIElement* UI::getelement(Element::UIType type) const
	{
		return state->get(type);
	}
}