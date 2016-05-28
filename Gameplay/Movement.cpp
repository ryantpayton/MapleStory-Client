//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2016 Daniel Allendorf                                        //
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
#include "Movement.h"

namespace jrc
{
	template <>
	Movement InPacket::read<Movement>()
	{
		Movement fragment;
		fragment.command = read_byte();
		switch (fragment.command)
		{
		case 0:
		case 5:
		case 17:
			fragment.type = Movement::_ABSOLUTE;
			fragment.xpos = read_short();
			fragment.ypos = read_short();
			fragment.lastx = read_short();
			fragment.lasty = read_short();
			fragment.fh = read_short();
			fragment.newstate = read_byte();
			fragment.duration = read_short();
			break;
		case 1:
		case 2:
		case 6:
		case 12:
		case 13:
		case 16:
			fragment.type = Movement::_RELATIVE;
			fragment.xpos = read_short();
			fragment.ypos = read_short();
			fragment.newstate = read_byte();
			fragment.duration = read_short();
			break;
		case 11:
			fragment.type = Movement::CHAIR;
			fragment.xpos = read_short();
			fragment.ypos = read_short();
			skip(2);
			fragment.newstate = read_byte();
			fragment.duration = read_short();
			break;
		case 15:
			fragment.type = Movement::JUMPDOWN;
			fragment.xpos = read_short();
			fragment.ypos = read_short();
			fragment.lastx = read_short();
			fragment.lasty = read_short();
			skip(2);
			fragment.fh = read_short();
			fragment.newstate = read_byte();
			fragment.duration = read_short();
			break;
		case 3:
		case 4:
		case 7:
		case 8:
		case 9:
		case 14:
			fragment.type = Movement::NONE;
			break;
		case 10:
			fragment.type = Movement::NONE;
			//change equip
			break;
		}
		return fragment;
	}
}