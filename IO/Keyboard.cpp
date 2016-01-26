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

#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#include "glfw3.h"

#define LEFTKEY GLFW_KEY_LEFT
#define RIGHTKEY GLFW_KEY_RIGHT
#define UPKEY GLFW_KEY_UP
#define DOWNKEY GLFW_KEY_DOWN
#define SHIFTKEY GLFW_KEY_LEFT_SHIFT
#define BACKKEY GLFW_KEY_BACKSPACE
#define RETURNKEY GLFW_KEY_ENTER
#define SPACEKEY GLFW_KEY_SPACE

#else
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN

#define LEFTKEY VK_LEFT
#define RIGHTKEY VK_RIGHT
#define UPKEY VK_UP
#define DOWNKEY VK_DOWN
#define SHIFTKEY VK_SHIFT
#define BACKKEY VK_BACK
#define RETURNKEY VK_RETURN
#define SPACEKEY VK_SPACE

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
#endif
	Keyboard::Keyboard()
	{
		keymap[LEFTKEY] = Keymapping(KT_ACTION, KA_LEFT);
		keymap[RIGHTKEY] = Keymapping(KT_ACTION, KA_RIGHT);
		keymap[UPKEY] = Keymapping(KT_ACTION, KA_UP);
		keymap[DOWNKEY] = Keymapping(KT_ACTION, KA_DOWN);

		textactions[BACKKEY] = KA_BACK;
		textactions[RETURNKEY] = KA_RETURN;
		textactions[SPACEKEY] = KA_SPACE;
	}

	int32_t Keyboard::shiftcode() const
	{
		return SHIFTKEY;
	}

	Keyboard::Keymapping Keyboard::gettextmapping(int32_t keycode, bool shift) const
	{
		if (textactions.count(keycode))
		{
			return Keymapping(KT_ACTION, textactions.at(keycode));
		}
		else if (keycode > 47 && keycode < 65)
		{
			return Keymapping(KT_NUMBER, keycode - (shift ? 15 : 0));
		}
		else if (keycode > 64 && keycode < 91)
		{
			return Keymapping(KT_LETTER, keycode + (shift ? 0 : 32));
		}
		else
		{
			switch (keycode)
			{
			case LEFTKEY:
			case RIGHTKEY:
			case UPKEY:
			case DOWNKEY:
				return keymap.at(keycode);
			default:
				return Keymapping(KT_NONE, 0);
			}
		}
	}

	void Keyboard::assign(uint8_t key, Keytype type, int32_t action)
	{
		Keymapping mapping = Keymapping(type, action);
		keymap[Keytable[key]] = mapping;
		maplekeys[key] = mapping;
	}

	const Keyboard::Keymapping* Keyboard::getmapping(int32_t keycode) const
	{
		return keymap.count(keycode) ? &keymap.at(keycode) : nullptr;
	}
}
