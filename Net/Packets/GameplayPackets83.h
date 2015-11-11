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
#include "MovementPacket.h"
#include "Net\SendOpcodes83.h"

namespace Net
{
	// A packet that requests the server to warp the player to a different map.
	class ChangeMapPacket83 : public OutPacket
	{
	public:
		ChangeMapPacket83(bool died, int32_t targetid, string targetp, bool usewheel) : OutPacket(CHANGEMAP)
		{
			writech(died);
			writeint(targetid);
			writestr(targetp);
			writech(0);
			writesh(usewheel ? 1 : 0);
		}
	};

	// A packet which updates the player's position with the server.
	class MovePlayerPacket83 : public MovementPacket
	{
	public:
		MovePlayerPacket83(const MovementInfo& movements) : MovementPacket(MOVE_PLAYER)
		{
			writelg(0);
			writech(0);
			writemoves(movements);
		}
	};
}