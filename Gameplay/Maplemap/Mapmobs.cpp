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

#include "Util\BinaryTree.h"

namespace Gameplay
{
	void MapMobs::draw(int8_t layer, Point<int16_t> viewpos, float alpha) const
	{
		MapObjects::draw(layer, viewpos, alpha);

		switch (layer)
		{
		case 7:
			for (auto& be : bulleteffects)
			{
				be.draw(viewpos, alpha);
			}
			for (auto& dn : damagenumbers)
			{
				dn.draw(viewpos, alpha);
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
		uint8_t mobcount = attack.mobcount;
		Point<int16_t> origin = attack.origin;
		rectangle2d<int16_t> range = attack.range;
		Attack::Direction direction = attack.direction;
		switch (direction)
		{
		case Attack::TOLEFT:
			range = rectangle2d<int16_t>(
				origin.x() + static_cast<int16_t>(range.l() * attack.hrange),
				origin.x() + range.r(),
				origin.y() + range.t(),
				origin.y() + range.b()
				);
			break;
		case Attack::TORIGHT:
			range = rectangle2d<int16_t>(
				origin.x() - range.r(),
				origin.x() - static_cast<int16_t>(range.l() * attack.hrange),
				origin.y() + range.t(),
				origin.y() + range.b()
				);
			break;
		}

		vector<int32_t> targets = findclosest(range, origin, mobcount);
		AttackResult result = attack;
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
		if (result.bullet)
		{
			Point<int16_t> origin = user.getposition();
			bool toleft = result.direction == Attack::TOLEFT;
			Animation animation = move.getbullet(result.bullet);
			Bullet bullet = Bullet(animation, origin, toleft);

			for (auto& line : result.damagelines)
			{
				int32_t oid = line.first;
				Optional<Mob> mob = getmob(oid);
				if (mob)
				{
					vector<DamageNumber> numbers = mob->placenumbers(line.second);
					Point<int16_t> head = mob->getheadpos();

					size_t i = 0;
					for (auto& number : numbers)
					{
						uint16_t delay = user.getattackdelay(i, result.speed);
						DamageEffect effect = DamageEffect(move, user.getlevel(), user.istwohanded(), number, toleft, line.second[i].first, oid, delay);
						bulleteffects.push_back(BulletEffect(bullet, head, effect));
						i++;
					}
				}
			}

			if (result.damagelines.size() == 0)
			{
				auto target = Point<int16_t>(toleft ? origin.x() - 400 : origin.x() + 400, origin.y() - 26);
				for (uint8_t i = 0; i < result.hitcount; i++)
				{
					uint16_t delay = user.getattackdelay(i, result.speed);
					DamageEffect effect = DamageEffect(move, 0, 0, DamageNumber(), false, 0, 0, delay);
					bulleteffects.push_back(BulletEffect(bullet, target, effect));
				}
			}
		}
		else
		{
			bool toleft = result.direction == Attack::TOLEFT;

			for (auto& line : result.damagelines)
			{
				int32_t oid = line.first;
				Optional<Mob> mob = getmob(oid);
				if (mob)
				{
					vector<DamageNumber> numbers = mob->placenumbers(line.second);

					size_t i = 0;
					for (auto& number : numbers)
					{
						uint16_t delay = user.getattackdelay(i, result.speed);
						DamageEffect effect = DamageEffect(move, user.getlevel(), user.istwohanded(), number, toleft, line.second[i].first, oid, delay);
						damageeffects.push_back(effect);

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
	/*vector<int32_t> MapMobs::findclose(rectangle2d<int16_t> range, uint8_t mobcount) const
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

	vector<int32_t> MapMobs::findclosest(rectangle2d<int16_t> range, Point<int16_t> origin, uint8_t mobcount) const
	{
		BinaryTree<int32_t, int16_t> distancetree;
		for (auto& object : objects)
		{
			auto mob = Optional<MapObject>(object.second.get())
				.reinterpret<Mob>();
			if (mob && mob->isalive() && mob->isinrange(range))
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
}
