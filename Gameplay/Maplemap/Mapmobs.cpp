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

#include "..\..\Constants.h"

#include <map>
#include <algorithm>

namespace jrc
{
	void MapMobs::draw(int8_t layer, double viewx, double viewy, float alpha) const
	{
		MapObjects::draw(layer, viewx, viewy, alpha);

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
				applyeffect(effect);
			}
			return apply;
		});

		bulleteffects.remove_if([&](BulletEffect& mb){
			Optional<Mob> mob = getmob(mb.gettarget());
			if (mob)
			{
				Point<int16_t> target = mob->getheadpos();
				bool apply = mb.update(target);
				if (apply)
				{
					applyeffect(mb.geteffect());
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

		MapObjects::update(physics);
	}

	void MapMobs::sendspawn(const MobSpawn& spawn)
	{
		Optional<Mob> mob = getmob(spawn.getoid());
		if (mob)
		{
			int8_t mode = spawn.getmode();
			if (mode > 0)
				mob->setcontrol(mode);

			mob->makeactive();
		}
		else
		{
			Mob* newmob = spawn.instantiate();
			add(newmob);
		}
	}

	void MapMobs::movemob(int32_t oid, Point<int16_t> start, const Movement& movement)
	{
		Optional<Mob> mob = getmob(oid);
		if (mob)
		{
			mob->sendmovement(start, movement);
		}
	}

	AttackResult MapMobs::sendattack(const Attack& attack)
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
		std::vector<int32_t> targets = findclosest(range, origin, mobcount);
		for (auto& target : targets)
		{
			Optional<Mob> mob = getmob(target);
			if (mob)
			{
				result.damagelines[target] = mob->calculatedamage(attack);
				result.mobcount++;
			}
		}
		return result;
	}

	void MapMobs::showresult(const Char& user, const SpecialMove& move, const AttackResult& result)
	{
		AttackUser attackuser = { 
			user.getskilllevel(move.getid()), 
			user.getlevel(), 
			user.istwohanded(),
			!result.toleft
		};
		if (result.bullet)
		{
			auto bullet = Bullet(
				move.getbullet(user, result.bullet), 
				user.getposition(),
				result.toleft
				);

			for (auto& line : result.damagelines)
			{
				int32_t oid = line.first;
				Optional<Mob> mob = getmob(oid);
				if (mob)
				{
					std::vector<DamageNumber> numbers = mob->placenumbers(line.second);
					Point<int16_t> head = mob->getheadpos();

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
							user.getattackdelay(i, result.speed)
							);
						bulleteffects.emplace_back(bullet, head, effect);
						i++;
					}
				}
			}

			if (result.damagelines.size() == 0)
			{
				int16_t xshift = result.toleft ? -400 : 400;
				Point<int16_t> target = user.getposition() + Point<int16_t>(xshift, -26);
				for (uint8_t i = 0; i < result.hitcount; i++)
				{
					auto effect = DamageEffect(
						move, 
						attackuser,
						DamageNumber(), 
						false, 
						0, 
						0, 
						user.getattackdelay(i, result.speed)
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
				Optional<Mob> mob = getmob(oid);
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
							user.getattackdelay(i, result.speed)
							);

						i++;
					}
				}
			}
		}
	}

	void MapMobs::applyeffect(const DamageEffect& effect)
	{
		Optional<Mob> mob = getmob(effect.gettarget());
		if (mob)
		{
			effect.apply(*mob);

			DamageNumber number = effect.getnumber();
			Point<int16_t> head = mob->getheadpos();
			number.setx(head.x());
			damagenumbers.push_back(number);
		}
	}

	// Not sure if I need this, maybe for AOE?
	/*vector<int32_t> MapMobs::findclose(Rectangle<int16_t> range, uint8_t mobcount) const
	{
		vector<int32_t> targets;
		for (auto& object : objects)
		{
			Optional<MapObject> mmo = object.second.get();
			if (mmo.isempty())
				continue;

			Optional<Mob> mob = mmo.reinterpret<Mob>();
			if (mob->isalive() && mob->isinrange(range))
			{
				int32_t oid = mob->getoid();
				targets.push_back(oid);

				if (targets.size() == mobcount)
					break;
			}
		}
		return targets;
	}*/

	std::vector<int32_t> MapMobs::findclosest(Rectangle<int16_t> range, Point<int16_t> origin, uint8_t mobcount) const
	{
		std::multimap<int16_t, int32_t> distances;
		for (auto& object : objects)
		{
			auto mob = Optional<MapObject>(object.second.get())
				.reinterpret<Mob>();
			if (mob && mob->isalive() && mob->isinrange(range))
			{
				int32_t oid = mob->getoid();
				int16_t distance = mob->getposition().distance(origin);
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

	void MapMobs::setcontrol(int32_t oid, bool control)
	{
		int8_t mode = control ? 1 : 0;
		getmob(oid)
			.ifpresent(&Mob::setcontrol, mode);
	}

	Optional<Mob> MapMobs::getmob(int32_t oid)
	{
		return get(oid)
			.reinterpret<Mob>();
	}

	Optional<const Mob> MapMobs::getmob(int32_t oid) const
	{
		return get(oid)
			.reinterpret<const Mob>();
	}
}
