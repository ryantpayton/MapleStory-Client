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
		keymap[GLFW_KEY_LEFT] = std::make_pair(KT_ACTION, KA_LEFT);
		keymap[GLFW_KEY_RIGHT] = std::make_pair(KT_ACTION, KA_RIGHT);
		keymap[GLFW_KEY_UP] = std::make_pair(KT_ACTION, KA_UP);
		keymap[GLFW_KEY_DOWN] = std::make_pair(KT_ACTION, KA_DOWN);
		keymap[GLFW_KEY_BACKSPACE] = std::make_pair(KT_ACTION, KA_BACK);
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
		keymap[VK_LEFT] = std::make_pair(KT_ACTION, KA_LEFT);
		keymap[VK_RIGHT] = std::make_pair(KT_ACTION, KA_RIGHT);
		keymap[VK_UP] = std::make_pair(KT_ACTION, KA_UP);
		keymap[VK_DOWN] = std::make_pair(KT_ACTION, KA_DOWN);
		keymap[VK_BACK] = std::make_pair(KT_ACTION, KA_BACK);
	}
#endif

	void Keyboard::setenabled(Keytype type, bool en)
	{
		enabledtypes[type] = en;
	}

	void Keyboard::addmapping(uint8_t key, Keytype type, int32_t action)
	{
		keymap[Keytable[key]] = std::make_pair(type, action);
		maplekeys[key] = std::make_pair(type, action);
	}

	void Keyboard::focustarget(Textfield* kt)
	{
		focused = kt;
	}

	void Keyboard::sendinput(bool down, int32_t key)
	{
		if (focused)
		{
			if (down)
			{
				focused->sendkey(KT_ACTION, keymap[key].second, down);
			}
			else if (key > 64 && key < 91)
			{
#ifdef JOURNEY_USE_OPENGL
				int8_t letter = keystate[GLFW_KEY_LEFT_SHIFT] ? static_cast<int8_t>(key) : static_cast<int8_t>(key + 32);
#else
				int8_t letter = keystate[VK_SHIFT] ? static_cast<int8_t>(key) : static_cast<int8_t>(key + 32);
#endif
				focused->sendkey(KT_LETTER, letter, down);
			}
		}
		else
		{
			Keytype type = keymap[key].first;
			int action = keymap[key].second;

			if (enabledtypes[type])
			{
				switch (type)
				{
				case KT_MENU:
					UI::sendkey(type, action, down);
					break;
				case KT_ACTION:
					Gameplay::Stage::sendkey(type, action, down);
					break;
				}
			}
		}

		keystate[key] = down;
	}
}
