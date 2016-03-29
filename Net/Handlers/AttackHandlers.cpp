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
#include "AttackHandlers.h"

#include "Gameplay\Combat\Skill.h"
#include "Gameplay\Stage.h"

#include <unordered_map>
#include <vector>

namespace Net
{
	AttackHandler::AttackHandler(Attack::Type t)
	{
		type = t;
	}

	void AttackHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.readint();
		uint8_t count = recv.readbyte();

		recv.skip(1);

		using Gameplay::AttackResult;
		AttackResult attack;
		attack.type = type;

		attack.level = recv.readbyte();
		attack.skill = (attack.level > 0) ? recv.readint() : 0;

		attack.display = recv.readbyte();
		attack.direction = recv.readbyte();
		attack.stance = recv.readbyte();
		attack.speed = recv.readbyte();

		recv.skip(1);

		attack.bullet = recv.readint();

		attack.mobcount = (count >> 4) & 0xF;
		attack.hitcount = count & 0xF;
		for (uint8_t i = 0; i < attack.mobcount; i++)
		{
			int32_t oid = recv.readint();

			recv.skip(1);

			using Gameplay::Skill;
			uint8_t length = (attack.skill == Skill::MESO_EXPLOSION) ? recv.readbyte() : attack.hitcount;
			for (uint8_t j = 0; j < length; j++)
			{
				int32_t damage = recv.readint();
				bool critical = false; // todo
				auto singledamage = std::make_pair(damage, critical);
				attack.damagelines[oid].push_back(singledamage);
			}
		}

		using Gameplay::Stage;
		Stage::get().showattack(cid, attack);
	}
}