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
#include "Constants.h"

#include "Net\Session.h"
#include "Net\Packets\AttackAndSkillPackets.h"

#include "Util\BinaryTree.h"

namespace Gameplay
{
	void MapMobs::update(const Physics& physics)
	{
		attacklist.remove_if([&](Attack& attack){
			bool apply = attack.update(Constants::TIMESTEP);
			if (apply)
			{
				applyattack(attack);
				return true;
			}
			else
			{
				return false;
			}
		});

		MapObjects::update(physics);
	}

	void MapMobs::applyattack(const Attack& attack)
	{
		uint8_t mobcount = attack.mobcount;
		if (mobcount == 0)
			return;

		Attack::Direction direction = attack.direction;
		Point<int16_t> origin = attack.origin;
		rectangle2d<int16_t> range = attack.range;
		switch (direction)
		{
		case Attack::TOLEFT:
			range = rectangle2d<int16_t>(
				range.getlt() + origin,
				range.getrb() + origin
				);
			break;
		case Attack::TORIGHT:
			range = rectangle2d<int16_t>(
				origin.x() - range.r(),
				origin.x() - range.l(),
				origin.y() + range.t(),
				origin.y() + range.b()
				);
			break;
		}

		vector<int32_t> targets;
		switch (attack.type)
		{
		case Attack::CLOSE:
			targets = findclose(range, mobcount);
			break;
		case Attack::RANGED:
			targets = findranged(range, origin, mobcount);
			break;
		}

		AttackResult result = AttackResult(attack);
		for (auto& target : targets)
		{
			Optional<Mob> mob = getmob(target);
			if (mob)
			{
				result.damagelines[target] = mob->damage(attack);
				result.mobcount++;
			}
		}

		attack.usesound.play();

		using Net::Session;
		switch (attack.type)
		{
		case Attack::CLOSE:
			using Net::CloseRangeAttackPacket;
			Session::get().dispatch(CloseRangeAttackPacket(result));
			break;
		case Attack::RANGED:
			using Net::RangedAttackPacket;
			Session::get().dispatch(RangedAttackPacket(result));
			break;
		}
	}

	vector<int32_t> MapMobs::findclose(rectangle2d<int16_t> range, uint8_t mobcount) const
	{
		vector<int32_t> targets;
		for (auto& object : objects)
		{
			Optional<MapObject> mmo = object.second.get();
			if (mmo.isempty())
				continue;

			Optional<Mob> mob = mmo.reinterpret<Mob>();
			if (mob->isactive() && mob->isinrange(range))
			{
				int32_t oid = mob->getoid();
				targets.push_back(oid);

				if (targets.size() == mobcount)
					break;
			}
		}
		return targets;
	}

	vector<int32_t> MapMobs::findranged(rectangle2d<int16_t> range, Point<int16_t> origin, uint8_t mobcount) const
	{
		BinaryTree<int32_t, int16_t> distancetree;
		for (auto& object : objects)
		{
			Optional<MapObject> mmo = object.second.get();
			if (mmo.isempty())
				continue;

			Optional<Mob> mob = mmo.reinterpret<Mob>();
			if (mob->isactive() && mob->isinrange(range))
			{
				int32_t oid = mob->getoid();
				int16_t distance = mob->getposition().distance(origin);
				distancetree.add(oid, distance);
			}
		}

		vector<int32_t> targets;
		auto collector = [&](int32_t oid){
			if (targets.size() < mobcount)
				targets.push_back(oid);
		};
		distancetree.minwalk(collector);
		return targets;
	}

	void MapMobs::killmob(int32_t oid, int8_t animation)
	{
		getmob(oid)
			.ifpresent(&Mob::kill, animation);
	}

	void MapMobs::sendmobhp(int32_t oid, int8_t percent, uint16_t playerlevel)
	{
		getmob(oid)
			.ifpresent(&Mob::sendhp, percent, playerlevel);
	}

	void MapMobs::sendattack(Attack attack)
	{
		attacklist.push_back(attack);
	}

	Optional<Mob> MapMobs::getmob(int32_t oid)
	{
		return get(oid)
			.reinterpret<Mob>();
	}
}
