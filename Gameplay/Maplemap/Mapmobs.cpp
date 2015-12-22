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
#include "MapMobs.h"
#include "Net\Session.h"
#include "Net\Packets\GameplayPackets83.h"

namespace Gameplay
{
	MapMobs::MapMobs() {}

	void MapMobs::addmob(int32_t oid, int32_t id, bool control, int8_t stance, 
		uint16_t fhid, bool fadein, int8_t team, vector2d<int16_t> position) {

		Mob* mob = getmob(oid);
		if (mob)
			mob->setactive(true);
		else
			add(new Mob(oid, id, control, stance, fhid, fadein, team, position));
	}

	void MapMobs::killmob(int32_t oid, int8_t animation)
	{
		Mob* mob = getmob(oid);
		if (mob)
		{
			mob->kill(animation);
		}
	}

	void MapMobs::sendmobhp(int32_t oid, int8_t percent, uint16_t playerlevel)
	{
		Mob* mob = getmob(oid);
		if (mob)
		{
			mob->sendhp(percent, playerlevel);
		}
	}

	AttackResult MapMobs::sendattack(const Attack& attack)
	{
		rectangle2d<int16_t> range;
		if (attack.direction == Attack::TOLEFT)
		{
			range = rectangle2d<int16_t>(
				attack.range.getlt() + attack.origin,
				attack.range.getrb() + attack.origin);
		}
		else if (attack.direction == Attack::TORIGHT)
		{
			range = rectangle2d<int16_t>(
				attack.origin.x() - attack.range.r(),
				attack.origin.x() - attack.range.l(),
				attack.origin.y() + attack.range.t(),
				attack.origin.y() + attack.range.b());
		}

		AttackResult result;
		for (auto& mmo : objects)
		{
			if (result.damagelines.size() == attack.mobcount)
				break;

			if (mmo.second == nullptr)
				continue;

			Mob* mob = reinterpret_cast<Mob*>(mmo.second.get());
			if (mob->isactive() && mob->isinrange(range))
				result.damagelines[mob->getoid()] = mob->damage(attack);
		}
		result.direction = static_cast<uint8_t>(attack.direction);
		result.mobcount = static_cast<uint8_t>(result.damagelines.size());
		result.hitcount = attack.hitcount;
		result.skill = attack.skill;
		result.speed = attack.speed;
		result.display = attack.delay;
		return result;
	}

	Mob* MapMobs::getmob(int32_t oid)
	{
		MapObject* mmo = get(oid);
		if (mmo)
			return reinterpret_cast<Mob*>(mmo);
		else
			return nullptr;
	}
}
