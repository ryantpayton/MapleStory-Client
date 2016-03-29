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
#include "Configuration.h"

namespace IO
{
	UIDragElement::UIDragElement(Settings::Type set, Point<int16_t> dga)
	{
		setting = set;
		dragarea = dga;

		dragged = false;
		position = Configuration::get().getpoint(setting);
	}

	bool UIDragElement::cursorleave(bool clicked, Point<int16_t> cursorpos)
	{
		if (dragged)
		{
			if (clicked)
			{
				position = cursorpos - cursoroffset;
				return true;
			}
			else
			{
				dragged = false;
				Configuration::get().setpoint(setting, position);
			}
		}
		return false;
	}

	Cursor::State UIDragElement::sendmouse(bool clicked, Point<int16_t> cursorpos)
	{
		if (clicked)
		{
			if (dragged)
			{
				position = cursorpos - cursoroffset;
				return Cursor::CLICKING;
			}
			else if (indragrange(cursorpos))
			{
				cursoroffset = cursorpos - position;
				dragged = true;
				return Cursor::CLICKING;
			}
		}
		else
		{
			if (dragged)
			{
				dragged = false;
				Configuration::get().setpoint(setting, position);
			}
		}
		return UIElement::sendmouse(clicked, cursorpos);
	}

	bool UIDragElement::indragrange(Point<int16_t> cursorpos) const
	{
		auto bounds = rectangle2d<int16_t>(position, position + dragarea);
		return bounds.contains(cursorpos);
	}
}