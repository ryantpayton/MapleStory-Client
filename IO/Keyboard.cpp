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
#include "IO\Keyboard.h"
#include "IO\UI.h"
#include "Gameplay\Stage.h"

#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#include "glfw3.h"
#else
#include <Windows.h>
#endif

namespace IO
{
#ifdef JOURNEY_USE_OPENGL
	const int32_t Keytable[90] =
	{
		0, 0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, // number keys, up to key 0
		0, 0, 0, 0, 'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', 0, 0, 0, //first letter row, up to key 28
		GLFW_KEY_LEFT_CONTROL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0, 0, 0,  //second row, up to 41
		GLFW_KEY_LEFT_SHIFT, 0, 'Y', 'X', 'C', 'V', 'B', 'N', 'M', 0, 0, 0, 0, 0, GLFW_KEY_MENU, GLFW_KEY_SPACE, 0, //third row, up to 58
		GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5, GLFW_KEY_F6, GLFW_KEY_F7, GLFW_KEY_F8, GLFW_KEY_F9, GLFW_KEY_F10, GLFW_KEY_F11, GLFW_KEY_F12, //up to 70
		GLFW_KEY_HOME, 0, GLFW_KEY_PAGE_UP, 0, 0, 0, 0, 0, GLFW_KEY_END, 0, GLFW_KEY_PAGE_DOWN, GLFW_KEY_INSERT, GLFW_KEY_DELETE, 0, 0, 0, 0, 0, 0
	};

	Keyboard::Keyboard()
	{
		Keymapping left;
		left.type = KT_ACTION;
		left.action = KA_LEFT;
		Keymapping right;
		right.type = KT_ACTION;
		right.action = KA_RIGHT;
		Keymapping up;
		up.type = KT_ACTION;
		up.action = KA_UP;
		Keymapping down;
		down.type = KT_ACTION;
		down.action = KA_DOWN;
		Keymapping back;
		back.type = KT_ACTION;
		back.action = KA_BACK;

		keymap[GLFW_KEY_LEFT] = left;
		keymap[GLFW_KEY_RIGHT] = right;
		keymap[GLFW_KEY_UP] = up;
		keymap[GLFW_KEY_DOWN] = down;
		keymap[GLFW_KEY_BACKSPACE] = back;
	}

	int32_t Keyboard::getshiftkeycode() const
	{
		return GLFW_KEY_LEFT_SHIFT;
	}

	Keyboard::Keymapping Keyboard::gettextmapping(int32_t keycode) const
	{
		switch (keycode)
		{
		case GLFW_KEY_LEFT:
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_UP:
		case GLFW_KEY_DOWN:
		case GLFW_KEY_BACKSPACE:
			return keymap.at(keycode);
		default:
			if (keycode > 47 && keycode < 58)
			{
				Keymapping mapping;
				mapping.type = KT_NUMBER;
				mapping.action = keycode;
				return mapping;
			}
			else if (keycode > 64 && keycode < 91)
			{
				Keymapping mapping;
				mapping.type = KT_LETTER;
				mapping.action = keycode;
				return mapping;
			}
			break;
		}

		return Keymapping();
	}
#else
	const int32_t Keytable[90] =
	{
		0, 0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, // number keys, up to key 0
		0, 0, 0, 0, 'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', 0, 0, 0, //first letter row, up to key 28
		VK_CONTROL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0, 0, 0,  //second row, up to 41
		VK_SHIFT, 0, 'Y', 'X', 'C', 'V', 'B', 'N', 'M', 0, 0, 0, 0, 0, VK_MENU, VK_SPACE, 0, //third row, up to 58
		VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12, //up to 70
		VK_HOME, 0, VK_PRIOR, 0, 0, 0, 0, 0, VK_END, 0, VK_NEXT, VK_INSERT, VK_DELETE, 0, 0, 0, 0, 0, 0
	};

	Keyboard::Keyboard()
	{
		Keymapping left;
		left.type = KT_ACTION;
		left.action = KA_LEFT;
		Keymapping right;
		right.type = KT_ACTION;
		right.action = KA_RIGHT;
		Keymapping up;
		up.type = KT_ACTION;
		up.action = KA_UP;
		Keymapping down;
		down.type = KT_ACTION;
		down.action = KA_DOWN;
		Keymapping back;
		back.type = KT_ACTION;
		back.action = KA_BACK;

		keymap[VK_LEFT] = left;
		keymap[VK_RIGHT] = right;
		keymap[VK_UP] = up;
		keymap[VK_DOWN] = down;
		keymap[VK_BACK] = back;
	}

	int32_t Keyboard::getshiftkeycode() const
	{
		return VK_SHIFT;
	}

	Keyboard::Keymapping Keyboard::gettextmapping(int32_t keycode) const
	{
		switch (keycode)
		{
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
		case VK_BACK:
			return keymap.at(keycode);
		default:
			if (keycode > 47 && keycode < 58)
			{
				Keymapping mapping;
				mapping.type = KT_NUMBER;
				mapping.action = keycode;
				return mapping;
			}
			else if (keycode > 64 && keycode < 91)
			{
				Keymapping mapping;
				mapping.type = KT_LETTER;
				mapping.action = keycode;
				return mapping;
			}
			break;
		}

		return Keymapping();
	}
#endif

	void Keyboard::addmapping(uint8_t key, Keytype type, int32_t action)
	{
		Keymapping mapping;
		mapping.type = type;
		mapping.action = action;

		keymap[Keytable[key]] = mapping;
		maplekeys[key] = mapping;
	}

	const Keyboard::Keymapping* Keyboard::getmapping(int32_t keycode) const
	{
		if (keymap.count(keycode))
			return &keymap.at(keycode);
		else
			return nullptr;
	}
}
