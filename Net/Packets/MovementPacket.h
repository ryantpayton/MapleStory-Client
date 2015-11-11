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
#include "Net\SendOpcodes83.h"
#include "Gameplay\MovementInfo.h"

namespace Net
{
	using::Gameplay::MovementInfo;
	using::Gameplay::MovementFragment;

	// Base class for packets which need to update object movements with the server.
	class MovementPacket : public OutPacket
	{
	public:
		MovementPacket(SendOpcode83 opc) : OutPacket(opc) {}
	protected:
		void writemoves(const MovementInfo& movement)
		{
			writech(static_cast<int8_t>(movement.getsize()));

			for (vector<MovementFragment>::const_iterator mvit = movement.getbegin(); mvit != movement.getend(); ++mvit)
			{
				writech(mvit->command);
				writesh(mvit->xpos);
				writesh(mvit->ypos);
				writesh(mvit->lastx);
				writesh(mvit->lasty);
				writesh(0);
				writech(mvit->newstate);
				writesh(mvit->duration);
			}
		}
	};
}