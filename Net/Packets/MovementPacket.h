/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "..\OutPacket.h"

#include "..\..\Gameplay\Movement.h"

namespace jrc
{
	// Base class for packets which update object movements with the server.
	class MovementPacket : public OutPacket
	{
	public:
		MovementPacket(OutPacket::Opcode opc) : OutPacket(opc) {}

	protected:
		void writemovement(const Movement& movement)
		{
			writech(movement.command);
			writesh(movement.xpos);
			writesh(movement.ypos);
			writesh(movement.lastx);
			writesh(movement.lasty);
			writesh(movement.fh);
			writech(movement.newstate);
			writesh(movement.duration);
		}
	};
}