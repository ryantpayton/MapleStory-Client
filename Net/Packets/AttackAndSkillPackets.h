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

#include "Gameplay\Combat\Attack.h"

namespace Net
{
	using Gameplay::Attack;
	using Gameplay::AttackResult;

	// Notifies the server of an attack. The opcode is determined by the attack type.
	// Attack::CLOSE = CLOSE_ATTACK(44)
	// Attack::RANGED = RANGED_ATTACK(45)
	// Attack::MAGIC = MAGIC_ATTACK(46)
	class AttackPacket : public OutPacket
	{
	public:
		AttackPacket(const AttackResult& attack) 
			: OutPacket(opcodefor(attack.type)) {

			skip(1);

			writech((attack.mobcount << 4) | attack.hitcount);
			writeint(attack.skill);
			if (attack.charge > 0)
				writeint(attack.charge);

			skip(8);

			writech(attack.display);
			writech(attack.toleft);
			writech(attack.stance);

			skip(1);

			writech(attack.speed);

			if (attack.type == Attack::RANGED)
			{
				skip(1);
				writech(attack.toleft);
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
					writeint(singledamage.first);
					// add critical here
				}

				if (attack.skill != 5221004)
					skip(4);
			}
		}

	private:
		static SendOpcode opcodefor(Attack::Type type)
		{
			switch (type)
			{
			case Attack::CLOSE:
				return CLOSE_ATTACK;
			case Attack::RANGED:
				return RANGED_ATTACK;
			default:
				return MAGIC_ATTACK;
			}
		}
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