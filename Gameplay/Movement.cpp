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

namespace Net
{
	template <>
	Movement InPacket::read<Movement>()
	{
		Movement fragment;
		fragment.command = readbyte();
		switch (fragment.command)
		{
		case 0:
		case 5:
		case 17:
			fragment.type = Movement::_ABSOLUTE;
			fragment.xpos = readshort();
			fragment.ypos = readshort();
			fragment.lastx = readshort();
			fragment.lasty = readshort();
			fragment.fh = readshort();
			fragment.newstate = readbyte();
			fragment.duration = readshort();
			break;
		case 1:
		case 2:
		case 6:
		case 12:
		case 13:
		case 16:
			fragment.type = Movement::_RELATIVE;
			fragment.xpos = readshort();
			fragment.ypos = readshort();
			fragment.newstate = readbyte();
			fragment.duration = readshort();
			break;
		case 11:
			fragment.type = Movement::CHAIR;
			fragment.xpos = readshort();
			fragment.ypos = readshort();
			skip(2);
			fragment.newstate = readbyte();
			fragment.duration = readshort();
			break;
		case 15:
			fragment.type = Movement::JUMPDOWN;
			fragment.xpos = readshort();
			fragment.ypos = readshort();
			fragment.lastx = readshort();
			fragment.lasty = readshort();
			skip(2);
			fragment.fh = readshort();
			fragment.newstate = readbyte();
			fragment.duration = readshort();
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