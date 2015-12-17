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
#include "MovementPacket83.h"
#include "Net\SendOpcodes.h"
#include "Gameplay\Attack.h"

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
			skip(1);
			writesh(usewheel ? 1 : 0);
		}
	};

	// A packet which updates the player's position with the server.
	class MovePlayerPacket83 : public MovementPacket83
	{
	public:
		MovePlayerPacket83(const MovementInfo& movements) : MovementPacket83(MOVE_PLAYER)
		{
			skip(9);
			writemoves(movements);
		}
	};

	// A packet which requests that an 'USE' item is used.
	class UseItemPacket83 : public OutPacket
	{
	public:
		UseItemPacket83(int16_t slot, int32_t itemid) : OutPacket(USE_ITEM)
		{
			writeint(0);
			writesh(slot);
			writeint(itemid);
		}
	};

	class CloseRangeAttackPacket83 : public OutPacket
	{
	public:
		CloseRangeAttackPacket83(const Gameplay::AttackResult& attack) : OutPacket(CLOSE_ATTACK)
		{
			skip(1);
			writech((attack.hitcount << 4) | attack.hitcount);
			writeint(attack.skill);
			if (attack.charge > 0)
				writeint(attack.charge);
			skip(8);
			writech(attack.display);
			writech(attack.direction);
			writech(attack.stance);
			skip(1);
			writech(attack.speed);
			skip(4);

			for (auto& damagetomob : attack.damagelines)
			{
				writeint(damagetomob.first);
				skip(14);

				for (auto& singledamage : damagetomob.second)
				{
					writeint(singledamage);
				}

				if (attack.skill != 5221004)
					skip(4);
			}
		}
	};
}