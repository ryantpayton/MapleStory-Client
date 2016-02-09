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
#include "Window.h"
#include "LoginElements.h"
#include "GameElements.h"

#include "UITypes\UIBuffList.h"
#include "UITypes\UIStatusbar.h"
#include "UITypes\UIStatsinfo.h"
#include "UITypes\UIItemInventory.h"
#include "UITypes\UIEquipInventory.h"
#include "UITypes\UINpcTalk.h"

#include "Gameplay\Stage.h"

#include "Net\Session.h"
#include "Net\Packets\InventoryPackets.h"

namespace IO
{
	using Gameplay::Stage;

	UI::UI()
	{
		focusedtextfield = nullptr;
		mode = LOGIN;
		enabled = true;
	}

	void UI::init()
	{
		elements = unique_ptr<UIElements>(new LoginElements());
	}

	void UI::draw(float inter) const
	{
		elements->draw(inter, cursor.getposition());
		cursor.draw(inter);
	}

	void UI::update()
	{
		elements->update();
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

	void UI::changemode(Mode md)
	{
		if (mode == md)
			return;

		switch (md)
		{
		case LOGIN:
			elements = unique_ptr<UIElements>(new LoginElements());
			break;
		case GAME:
			elements = unique_ptr<UIElements>(new GameElements());
			break;
		}

		mode = md;
	}

	void UI::sendmouse(bool pressed, vector2d<int16_t> pos)
	{
		Cursor::Mousestate state = pressed ? Cursor::MST_CLICKING : Cursor::MST_IDLE;
		state = elements->sendmouse(state, pos);

		cursor.setposition(pos);
		cursor.setstate(state);
	}

	void UI::sendmouse(vector2d<int16_t> pos)
	{
		Cursor::Mousestate state = elements->sendmouse(cursor.getstate(), pos);

		cursor.setposition(pos);
		cursor.setstate(state);
	}

	void UI::doubleclick(vector2d<int16_t> pos)
	{
		elements->doubleclick(pos);
	}

	void UI::addelementbykey(int32_t key)
	{
		switch (key)
		{
		case Keyboard::CHARSTATS:
			add(ElementStatsinfo());
			break;
		case Keyboard::INVENTORY:
			add(ElementItemInventory());
			break;
		case Keyboard::EQUIPS:
			add(ElementEquipInventory());
			break;
		}
	}

	void UI::sendkey(int32_t keycode, bool pressed)
	{
		if (focusedtextfield)
		{
			Keyboard::Keymapping mapping = keyboard.gettextmapping(keycode, keydown[keyboard.shiftcode()]);

			switch (mapping.type)
			{
			case Keyboard::ACTION:
				focusedtextfield->sendkey(Keyboard::ACTION, mapping.action, pressed);
				break;
			case Keyboard::LETTER:
				focusedtextfield->sendkey(Keyboard::LETTER, static_cast<int8_t>(mapping.action), pressed);
				break;
			case Keyboard::NUMBER:
				focusedtextfield->sendkey(Keyboard::NUMBER, static_cast<int8_t>(mapping.action), pressed);
				break;
			}
		}
		else if (mode == GAME)
		{
			const Keyboard::Keymapping* mapping = keyboard.getmapping(keycode);

			if (mapping)
			{
				Keyboard::Keytype type = mapping->type;
				int32_t action = mapping->action;

				switch (type)
				{
				case Keyboard::MENU:
					if (pressed)
						addelementbykey(action);
					break;
				case Keyboard::ACTION:
				case Keyboard::FACE:
				case Keyboard::ITEM:
				case Keyboard::SKILL:
					Stage::get().sendkey(type, action, pressed);
					break;
				}
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
		if (mode != GAME)
			return;

		GameElements* gameelements = reinterpret_cast<GameElements*>(elements.get());
		if (gameelements)
			gameelements->dragicon(icon);
	}

	void UI::addkeymapping(uint8_t no, uint8_t t, int32_t action)
	{
		Keyboard::Keytype type = static_cast<Keyboard::Keytype>(t);
		keyboard.assign(no, type, action);
	}

	void UI::add(const Element& element)
	{
		elements->add(element);
	}

	void UI::remove(Element::UIType type)
	{
		focusedtextfield = nullptr;
		elements->remove(type);
	}

	bool UI::haselement(Element::UIType type) const
	{
		return getelement(type) != nullptr;
	}

	UIElement* UI::getelement(Element::UIType type) const
	{
		return elements->get(type);
	}
}