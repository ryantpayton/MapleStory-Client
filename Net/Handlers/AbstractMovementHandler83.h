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
#pragma once
#include "Net\PacketHandler.h"
#include "Gameplay\MovementInfo.h"

namespace Net
{
	using::Gameplay::MovementFragment;

	class AbstractMovementHandler83 : public PacketHandler
	{
	protected:
		MovementFragment parsemovement(InPacket& recv) const
		{
			MovementFragment fragment;

			fragment.command = recv.readbyte();
			switch (fragment.command)
			{
			case 0:
			case 5:
			case 17:
				fragment.type = MovementFragment::MVT_ABSOLUTE;
				fragment.xpos = recv.readshort();
				fragment.ypos = recv.readshort();
				fragment.lastx = recv.readshort();
				fragment.lasty = recv.readshort();
				recv.skip(2);
				fragment.newstate = recv.readbyte();
				fragment.duration = recv.readshort();
				break;
			case 1:
			case 2:
			case 6:
			case 12:
			case 13:
			case 16:
				fragment.type = MovementFragment::MVT_RELATIVE;
				fragment.xpos = recv.readshort();
				fragment.ypos = recv.readshort();
				fragment.newstate = recv.readbyte();
				fragment.duration = recv.readshort();
				break;
			case 11:
				fragment.type = MovementFragment::MVT_CHAIR;
				fragment.xpos = recv.readshort();
				fragment.ypos = recv.readshort();
				recv.skip(2);
				fragment.newstate = recv.readbyte();
				fragment.duration = recv.readshort();
				break;
			case 15:
				fragment.type = MovementFragment::MVT_JUMPDOWN;
				fragment.xpos = recv.readshort();
				fragment.ypos = recv.readshort();
				fragment.lastx = recv.readshort();
				fragment.lasty = recv.readshort();
				recv.skip(2);
				fragment.fh = recv.readshort();
				fragment.newstate = recv.readbyte();
				fragment.duration = recv.readshort();
				break;
			case 3:
			case 4:
			case 7:
			case 8:
			case 9:
			case 14:
				fragment.type = MovementFragment::MVT_NONE;
				break;
			case 10:
				fragment.type = MovementFragment::MVT_NONE;
				//change equip
				break;
			}

			return fragment;
		}
	};
}