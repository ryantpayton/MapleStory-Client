/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "InputHandlerWM.h"

namespace IO
{
	void InputHandlerWM::sendkey(bool down, uint8_t key)
	{
		if (activetext)
		{
			if (down)
			{
				switch (key)
				{
				case VK_RETURN:
					break;
				case VK_TAB:
					break;
				case VK_BACK:
					activetext->sendinput(0);
					break;
				case VK_LEFT:
					activetext->sendinput(1);
					break;
				case VK_RIGHT:
					activetext->sendinput(2);
					break;
				}
			}
			else
			{
				if (key > 64 && key < 91)
				{
					char letter = keystate[VK_SHIFT] ? static_cast<char>(key) : static_cast<char>(key + 32);
					activetext->sendinput(letter);
				}
			}
		}

		keystate[key] = down;
	}

	void InputHandlerWM::focustext(Textfield* at)
	{
		if (activetext != 0)
		{
			activetext->setstate(TXFS_NORMAL);
		}
		activetext = at;
		if (activetext != 0)
		{
			activetext->setstate(TXFS_FOCUSED);
		}
	}
}
