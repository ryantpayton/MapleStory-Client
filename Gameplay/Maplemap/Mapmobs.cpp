/////////////////////////////////////////////////////////////////////////////
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

#include "..\..\Constants.h"

#include <map>
#include <algorithm>

namespace jrc
{
	void MapMobs::draw(int8_t layer, double viewx, double viewy, float alpha) const
	{
		mobs.draw(layer, viewx, viewy, alpha);

		switch (layer)
		{
		case 7:
			for (auto& be : bulleteffects)
			{
				be.draw(viewx, viewy, alpha);
			}
			for (auto& dn : damagenumbers)
			{
				dn.draw(viewx, viewy, alpha);
			}
			break;
		}
	}

	void MapMobs::update(const Physics& physics)
	{
		damageeffects.remove_if([&](DamageEffect& effect){
			bool apply = effect.update();
			if (apply)
			{
				apply_effect(effect);
			}
			return apply;
		});

		bulleteffects.remove_if([&](BulletEffect& mb){
			Optional<Mob> mob = get_mob(mb.get_target());
			if (mob)
			{
				Point<int16_t> target = mob->get_headpos();
				bool apply = mb.update(target);
				if (apply)
				{
					apply_effect(mb.get_effect());
				}
				return apply;
			}
			else
			{
				return mb.update();
			}
		});

		damagenumbers.remove_if([](DamageNumber& dn){
			return dn.update();
		});

		mobs.update(physics);
	}

	void MapMobs::send_spawn(const MobSpawn& spawn)
	{
		Optional<Mob> mob = get_mob(spawn.get_oid());
		if (mob)
		{
			int8_t mode = spawn.getmode();
			if (mode > 0)
				mob->set_control(mode);

			mob->makeactive();
		}
		else
		{
			mobs.add(
				spawn.instantiate()
			);
		}
	}

	void MapMobs::send_movement(int32_t oid, Point<int16_t> start, const Movement& movement)
	{
		Optional<Mob> mob = get_mob(oid);
		if (mob)
		{
			mob->send_movement(start, movement);
		}
	}

	AttackResult MapMobs::send_attack(const Attack& attack)
	{
		Point<int16_t> origin = attack.origin;
		Rectangle<int16_t> range = attack.range;
		int16_t hrange = static_cast<int16_t>(range.l() * attack.hrange);
		if (attack.toleft)
		{
			range = Rectangle<int16_t>(
				origin.x() + hrange,
				origin.x() + range.r(),
				origin.y() + range.t(),
				origin.y() + range.b()
				);
		}
		else
		{
			range = Rectangle<int16_t>(
				origin.x() - range.r(),
				origin.x() - hrange,
				origin.y() + range.t(),
				origin.y() + range.b()
				);
		}

		uint8_t mobcount = attack.mobcount;
		AttackResult result = attack;
		std::vector<int32_t> targets = find_closest(range, origin, mobcount);
		for (auto& target : targets)
		{
			Optional<Mob> mob = get_mob(target);
			if (mob)
			{
				result.damagelines[target] = mob->calculatedamage(attack);
				result.mobcount++;
			}
		}
		return result;
	}

	void MapMobs::show_result(const Char& user, const SpecialMove& move, const AttackResult& result)
	{
		AttackUser attackuser = { 
			user.get_skilllevel(move.get_id()), 
			user.get_level(), 
			user.istwohanded(),
			!result.toleft
		};
		if (result.bullet)
		{
			auto bullet = Bullet(
				move.get_animation(user, result.bullet), 
				user.get_position(),
				result.toleft
				);

			for (auto& line : result.damagelines)
			{
				int32_t oid = line.first;
				Optional<Mob> mob = get_mob(oid);
				if (mob)
				{
					std::vector<DamageNumber> numbers = mob->placenumbers(line.second);
					Point<int16_t> head = mob->get_headpos();

					size_t i = 0;
					for (auto& number : numbers)
					{
						auto effect = DamageEffect(
							move, 
							attackuser,
							number, 
							result.toleft,
							line.second[i].first, 
							oid, 
							user.get_attackdelay(i, result.speed)
							);
						bulleteffects.emplace_back(bullet, head, effect);
						i++;
					}
				}
			}

			if (result.damagelines.size() == 0)
			{
				int16_t xshift = result.toleft ? -400 : 400;
				Point<int16_t> target = user.get_position() + Point<int16_t>(xshift, -26);
				for (uint8_t i = 0; i < result.hitcount; i++)
				{
					auto effect = DamageEffect(
						move, 
						attackuser,
						DamageNumber(), 
						false, 
						0, 
						0, 
						user.get_attackdelay(i, result.speed)
						);
					bulleteffects.emplace_back(bullet, target, effect);
				}
			}
		}
		else
		{
			for (auto& line : result.damagelines)
			{
				int32_t oid = line.first;
				Optional<Mob> mob = get_mob(oid);
				if (mob)
				{
					std::vector<DamageNumber> numbers = mob->placenumbers(line.second);

					size_t i = 0;
					for (auto& number : numbers)
					{
						damageeffects.emplace_back(
							move, 
							attackuser,
							number, 
							result.toleft,
							line.second[i].first, 
							oid, 
							user.get_attackdelay(i, result.speed)
							);

						i++;
					}
				}
			}
		}
	}

	void MapMobs::apply_effect(const DamageEffect& effect)
	{
		Optional<Mob> mob = get_mob(effect.get_target());
		if (mob)
		{
			effect.apply(*mob);

			DamageNumber number = effect.get_number();
			Point<int16_t> head = mob->get_headpos();
			number.set_x(head.x());
			damagenumbers.push_back(number);
		}
	}

	std::vector<int32_t> MapMobs::find_closest(Rectangle<int16_t> range, Point<int16_t> origin, uint8_t mobcount) const
	{
		std::multimap<int16_t, int32_t> distances;
		for (auto& mmo : mobs)
		{
			const Mob* mob = static_cast<const Mob*>(mmo.second.get());
			if (mob && mob->isalive() && mob->is_in_range(range))
			{
				int32_t oid = mob->get_oid();
				int16_t distance = mob->get_position().distance(origin);
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

	void MapMobs::kill_mob(int32_t oid, int8_t animation)
	{
		get_mob(oid)
			.if_present(&Mob::kill, animation);
	}

	void MapMobs::send_mobhp(int32_t oid, int8_t percent, uint16_t playerlevel)
	{
		get_mob(oid)
			.if_present(&Mob::sendhp, percent, playerlevel);
	}

	void MapMobs::set_control(int32_t oid, bool control)
	{
		int8_t mode = control ? 1 : 0;
		get_mob(oid)
			.if_present(&Mob::set_control, mode);
	}

	void MapMobs::clear()
	{
		mobs.clear();
	}

	Optional<Mob> MapMobs::get_mob(int32_t oid)
	{
		return mobs.get(oid)
			.reinterpret<Mob>();
	}

	Optional<const Mob> MapMobs::get_mob(int32_t oid) const
	{
		return mobs.get(oid)
			.reinterpret<const Mob>();
	}
}
