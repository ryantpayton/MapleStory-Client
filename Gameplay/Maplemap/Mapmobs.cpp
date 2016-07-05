//////////////////////////////////////////////////////////////////////////////
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
#include "MapMobs.h"

#include "Mob.h"

#include <algorithm>
#include <iostream>
#include <map>

namespace jrc
{
	void MapMobs::draw(Layer::Id layer, double viewx, double viewy, float alpha) const
	{
		mobs.draw(layer, viewx, viewy, alpha);
	}

	void MapMobs::update(const Physics& physics)
	{
		for (; !spawns.empty(); spawns.pop())
		{
			const MobSpawn& spawn = spawns.front();

			if (Optional<Mob> mob = mobs.get(spawn.get_oid()))
			{
				int8_t mode = spawn.get_mode();
				if (mode > 0)
				{
					mob->set_control(mode);
				}
				mob->makeactive();
			}
			else
			{
				mobs.add(
					spawn.instantiate()
				);
			}
		}

		mobs.update(physics);
	}

	void MapMobs::spawn(MobSpawn&& spawn)
	{
		spawns.emplace(
			std::move(spawn)
		);
	}

	void MapMobs::remove(int32_t oid, int8_t animation)
	{
		if (Optional<Mob> mob = mobs.get(oid))
		{
			mob->kill(animation);
		}
	}

	void MapMobs::clear()
	{
		mobs.clear();
	}

	void MapMobs::set_control(int32_t oid, bool control)
	{
		int8_t mode = control ? 1 : 0;
		if (Optional<Mob> mob = mobs.get(oid))
		{
			mob->set_control(mode);
		}
	}

	void MapMobs::send_mobhp(int32_t oid, int8_t percent, uint16_t playerlevel)
	{
		if (Optional<Mob> mob = mobs.get(oid))
		{
			mob->show_hp(percent, playerlevel);
		}
	}

	void MapMobs::send_movement(int32_t oid, Point<int16_t> start, std::vector<Movement>&& movements)
	{
		if (Optional<Mob> mob = mobs.get(oid))
		{
			mob->send_movement(
				start, 
				std::move(movements)
			);
		}
	}

	AttackResult MapMobs::send_attack(const Attack& attack)
	{
		Point<int16_t> origin = attack.origin;
		Rectangle<int16_t> range = attack.range;
		int16_t hrange = static_cast<int16_t>(range.l() * attack.hrange);
		if (attack.toleft)
		{
			range = {
				origin.x() + hrange,
				origin.x() + range.r(),
				origin.y() + range.t(),
				origin.y() + range.b()
			};
		}
		else
		{
			range = {
				origin.x() - range.r(),
				origin.x() - hrange,
				origin.y() + range.t(),
				origin.y() + range.b()
			};
		}

		uint8_t mobcount = attack.mobcount;
		AttackResult result = attack;
		std::vector<int32_t> targets = find_closest(range, origin, mobcount);
		for (auto& target : targets)
		{
			if (Optional<Mob> mob = mobs.get(target))
			{
				result.damagelines[target] = mob->calculate_damage(attack);
				result.mobcount++;

				if (result.mobcount == 1)
				{
					result.first_oid = target;
				}
				if (result.mobcount == mobcount)
				{
					result.last_oid = target;
				}
			}
		}
		return result;
	}

	void MapMobs::apply_damage(int32_t oid, int32_t damage, bool toleft,
		const AttackUser& user, const SpecialMove& move) {

		if (Optional<Mob> mob = mobs.get(oid))
		{
			mob->apply_damage(damage, toleft);

			// maybe move this into the method above too?
			move.apply_hiteffects(user, *mob);
		}
	}

	std::vector<int32_t> MapMobs::find_closest(Rectangle<int16_t> range,
		Point<int16_t> origin, uint8_t mobcount) const {

		std::multimap<uint16_t, int32_t> distances;
		for (auto& mmo : mobs)
		{
			const Mob* mob = static_cast<const Mob*>(mmo.second.get());
			if (mob && mob->is_alive() && mob->is_in_range(range))
			{
				int32_t oid = mob->get_oid();
				uint16_t distance = mob->get_position().distance(origin);
				distances.emplace(distance, oid);
			}
		}

		std::vector<int32_t> targets;
		for (auto& iter : distances)
		{
			if (targets.size() >= mobcount)
				break;

			targets.push_back(iter.second);
		}
		return targets;
	}

	bool MapMobs::contains(int32_t oid) const
	{
		return mobs.contains(oid);
	}

	int32_t MapMobs::find_colliding(const MovingObject& moveobj) const
	{
		Range<int16_t> horizontal{ moveobj.get_last_x(), moveobj.get_x() };
		Range<int16_t> vertical{ moveobj.get_last_y(), moveobj.get_y() };
		Rectangle<int16_t> player_rect{ 
			horizontal.smaller(),
			horizontal.greater(),
			vertical.smaller() - 50,
			vertical.greater()
		};

		auto iter = std::find_if(mobs.begin(), mobs.end(), [&player_rect](auto& mmo){
			Optional<Mob> mob = mmo.second.get();
			return mob && mob->is_alive() && mob->is_in_range(player_rect);
		});
		if (iter == mobs.end())
			return 0;
		return iter->second->get_oid();
	}

	MobAttack MapMobs::create_attack(int32_t oid) const
	{
		if (Optional<const Mob> mob = mobs.get(oid))
		{
			return mob->create_touch_attack();
		}
		else
		{
			return{};
		}
	}

	Point<int16_t> MapMobs::get_mob_position(int32_t oid) const
	{
		if (auto mob = mobs.get(oid))
		{
			return mob->get_position();
		}
		else
		{
			return{};
		}
	}

	Point<int16_t> MapMobs::get_mob_head_position(int32_t oid) const
	{
		if (Optional<const Mob> mob = mobs.get(oid))
		{
			return mob->get_head_position();
		}
		else
		{
			return{};
		}
	}
}
