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
#include "Net\Packets\MovementPacket.h"

namespace Net
{
	// Requests the server to warp the player to a different map.
	// Opcode: CHANGE_MAP(38)
	class ChangeMapPacket : public OutPacket
	{
	public:
		ChangeMapPacket(bool died, int32_t targetid, string targetp, bool usewheel) : OutPacket(CHANGEMAP)
		{
			writech(died);
			writeint(targetid);
			writestr(targetp);
			skip(1);
			writesh(usewheel ? 1 : 0);
		}
	};


	// Updates the player's position with the server.
	// Opcode: MOVE_PLAYER(41)
	class MovePlayerPacket : public MovementPacket
	{
	public:
		MovePlayerPacket(const vector<MovementFragment>& movements) : MovementPacket(MOVE_PLAYER)
		{
			skip(9);
			writech(static_cast<int8_t>(movements.size()));
			for (auto& mv : movements)
			{
				writemovement(mv);
			}
		}
	};


	// Updates the a mob's position with the server.
	// Opcode: MOVE_MONSTER(188)
	class MoveMobPacket : public MovementPacket
	{
	public:
		MoveMobPacket(int32_t oid, int16_t type, int8_t skillb, int8_t skill0, int8_t skill1,
			int8_t skill2, int8_t skill3, int8_t skill4, vector2d<int16_t> startpos,
			const vector<MovementFragment>& movements) : MovementPacket(MOVE_MONSTER) {

			writeint(oid);
			writesh(type);
			writech(skillb);
			writech(skill0);
			writech(skill1);
			writech(skill2);
			writech(skill3);
			writech(skill4);
			writelg(0);
			writech(0);
			writeint(0);
			writesh(startpos.x());
			writesh(startpos.y());

			writech(static_cast<int8_t>(movements.size()));
			for (auto& mv : movements)
			{
				writemovement(mv);
			}
		}
	};


	// Requests picking up an item.
	// Opcode: PICKUP_ITEM(202)
	class PickupItemPacket : public OutPacket
	{
	public:
		PickupItemPacket(int32_t oid, vector2d<int16_t> position) : OutPacket(PICKUP_ITEM)
		{
			writeint(0);
			writech(0);
			writepoint(position);
			writeint(oid);
		}
	};
}