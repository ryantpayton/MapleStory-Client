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
#pragma once
#include "Cursor.h"
#include "nx.hpp"

namespace IO
{
	Cursor::Cursor()
	{
		position = vector2d<int>();
		state = MST_IDLE;
	}

	void Cursor::init()
	{
		node cursornode = nx::ui["Basic.img"]["Cursor"];

		for (Mousestate i = MST_IDLE; i <= MST_RCLICK; i = static_cast<Mousestate>(i + 1))
		{
			animations.add(i, new Animation(cursornode[to_string(i)]));
		}
	}

	void Cursor::draw()
	{
		animations.get(state)->draw(PosArgument(position));
	}

	void Cursor::update(short dpf)
	{
		animations.get(state)->update(dpf);
	}
}