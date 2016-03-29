//////////////////////////////////////////////////////////////////////////////
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
#include "MapObjects.h"
#include "Mob.h"
#include "Gameplay\Combat\Attack.h"
#include "Gameplay\Combat\SpecialMove.h"
#include "Gameplay\Spawn.h"
#include <list>

namespace Gameplay
{
	using std::list;

	class MapMobs : public MapObjects
	{
	public:
		void draw(int8_t layer, Point<int16_t> viewpos, float alpha) const override;
		void update(const Physics& physics) override;

		void sendspawn(const MobSpawn& spawn);
		void movemob(int32_t oid, Point<int16_t> start, const Movement& movement);
		void killmob(int32_t oid, int8_t effect);
		void sendmobhp(int32_t oid, int8_t percent, uint16_t playerlevel);
		void setcontrol(int32_t oid, bool control);

		AttackResult sendattack(Attack attack);
		void showresult(const Char& user, const SpecialMove& skill, const AttackResult& result);

	private:
		vector<int32_t> findclosest(rectangle2d<int16_t> range, Point<int16_t> origin, uint8_t mobcount) const;
		Optional<Mob> getmob(int32_t oid);

		list<DamageNumber> damagenumbers;

		class DamageEffect
		{
		public:
			DamageEffect(const SpecialMove& m, uint16_t l, bool th, DamageNumber n,
				bool tl, int32_t dm, int32_t t, uint16_t d)
				: move(m), level(l), twohanded(th), number(n), toleft(tl), damage(dm), target(t), delay(d) {}

			void apply(Mob& target) const
			{
				move.applyhiteffects(target, level, twohanded);

				target.applydamage(damage, toleft);
			}

			bool expired() const
			{
				return delay <= Constants::TIMESTEP;
			}

			bool update()
			{
				if (delay <= Constants::TIMESTEP)
				{
					return true;
				}
				else
				{
					delay -= Constants::TIMESTEP;
					return false;
				}
			}

			int32_t gettarget() const
			{
				return target;
			}

			DamageNumber getnumber() const
			{
				return number;
			}

		private:
			const DamageEffect& operator =(const DamageEffect&) = delete;

			const SpecialMove& move;
			DamageNumber number;
			uint16_t level;
			bool twohanded;
			int32_t damage;
			bool toleft;
			int32_t target;
			uint16_t delay;
		};
		list<DamageEffect> damageeffects;

		void applyeffect(const DamageEffect& effect);

		class BulletEffect
		{
		public:
			BulletEffect(Bullet b, Point<int16_t> t, DamageEffect de)
				: bullet(b), target(t), damageeffect(de) {}

			void draw(Point<int16_t> viewpos, float alpha) const
			{
				if (damageeffect.expired())
				{
					bullet.draw(viewpos, alpha);
				}
			}

			bool update()
			{
				bool expired = damageeffect.update();
				if (expired)
				{
					return bullet.update(target);
				}
				else
				{
					return false;
				}
			}

			bool update(Point<int16_t> newtarget)
			{
				target = newtarget;

				bool expired = damageeffect.update();
				if (expired)
				{
					return bullet.update(target);
				}
				else
				{
					return false;
				}
			}

			int32_t gettarget() const
			{
				return damageeffect.gettarget();
			}

			const DamageEffect& geteffect() const
			{
				return damageeffect;
			}

		private:
			const BulletEffect& operator =(const BulletEffect&) = delete;

			Bullet bullet;
			Point<int16_t> target;
			DamageEffect damageeffect;
		};
		list<BulletEffect> bulleteffects;
	};
}

