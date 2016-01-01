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
#include "UIDragElement.h"
#include "Program\Configuration.h"

namespace IO
{
	UIDragElement::UIDragElement(string cfn, vector2d<int16_t> dga)
	{
		configname = cfn;
		dragarea = dga;

		dragged = false;
		position = Program::Configuration::getvector2d(configname);
	}

	Cursor::Mousestate UIDragElement::sendmouse(bool pressed, vector2d<int16_t> cursorpos)
	{
		if (pressed)
		{
			if (dragged)
			{
				position = cursorpos - cursoroffset;
				return Cursor::MST_CLICKING;
			}
			else if (rectangle2d<int16_t>(position, position + dragarea).contains(cursorpos))
			{
				cursoroffset = cursorpos - position;
				dragged = true;
				return Cursor::MST_CLICKING;
			}
		}
		else
		{
			if (dragged)
			{
				dragged = false;
				Program::Configuration::setstring(configname, position.tostring());
			}
		}
		return UIElement::sendmouse(pressed, cursorpos);
	}
}