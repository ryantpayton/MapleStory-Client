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

#include "Gameplay\Attack.h"

namespace Net
{
	using Gameplay::AttackResult;

	// Base class for attack packets.
	class AttackPacket : public OutPacket
	{
	protected:
		AttackPacket(SendOpcode opc, const AttackResult& attack) : OutPacket(opc)
		{
			skip(1);
			writech((attack.mobcount << 4) | attack.hitcount);
			writeint(attack.skill);
			if (attack.charge > 0)
				writeint(attack.charge);
			skip(8);
			writech(attack.display);
			writech(attack.direction);
			writech(attack.stance);
			skip(1);
			writech(attack.speed);

			if (attack.ranged)
			{
				skip(1);
				writech(attack.direction);
				skip(7);
				// skip(4); if hurricane, piercing arrow or rapidfire
			}
			else
			{
				skip(4);
			}

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


	// Packet which notifies the server of a close-range attack.
	// Opcode: CLOSE_ATTACK(44)
	class CloseRangeAttackPacket : public AttackPacket
	{
	public:
		CloseRangeAttackPacket(const AttackResult& attack) : AttackPacket(CLOSE_ATTACK, attack) {}
	};


	// Packet which notifies the server of a ranged attack.
	// Opcode: RANGED_ATTACK(45)
	class RangedAttackPacket : public AttackPacket
	{
	public:
		RangedAttackPacket(const AttackResult& attack) : AttackPacket(RANGED_ATTACK, attack) {}
	};


	// Packet which notifies the server of a magic attack.
	// Opcode: MAGIC_ATTACK(46)
	class MagicAttackPacket : public AttackPacket
	{
	public:
		MagicAttackPacket(const AttackResult& attack) : AttackPacket(MAGIC_ATTACK, attack) {}
	};


	// Packet which notifies the server of a skill usage.
	// Opcode: USE_SKILL(91)
	class UseSkillPacket : public OutPacket
	{
	public:
		UseSkillPacket(int32_t skillid, int32_t level) : OutPacket(USE_SKILL)
		{
			writetime();
			writeint(skillid);
			writech(static_cast<uint8_t>(level));

			// if monster magnet : some more bytes

			if (skillid % 10000000 == 1004)
				skip(2); // no idea what this could be

			// a point (4 bytes) could be added at the end
		}
	};
}