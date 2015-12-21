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
#include "Net\OutPacket.h"
#include "Net\SendOpcodes.h"
#include "Gameplay\MovementInfo.h"

namespace Net
{
	using Gameplay::MovementFragment;

	// Base class for packets which update object movements with the server.
	class MovementPacket83 : public OutPacket
	{
	public:
		MovementPacket83(SendOpcode opc) : OutPacket(opc) {}
	protected:
		void writemoves(const MovementFragment& movement)
		{
			writech(1);
			writech(movement.command);
			writesh(movement.xpos);
			writesh(movement.ypos);
			writesh(movement.lastx);
			writesh(movement.lasty);
			writesh(0);
			writech(movement.newstate);
			writesh(movement.duration);
		}
	};
}