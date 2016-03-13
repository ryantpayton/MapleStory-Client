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
#include "Gameplay\Attack.h"
#include "Gameplay\Spawn.h"
#include <list>

namespace Gameplay
{
	using std::list;
	using std::pair;

	class MapMobs : public MapObjects
	{
	public:
		void draw(int8_t layer, Point<int16_t> viewpos, float alpha) const override;
		void update(const Physics& physics) override;

		void sendspawn(const MobSpawn& spawn);
		void killmob(int32_t oid, int8_t effect);
		void sendmobhp(int32_t oid, int8_t percent, uint16_t playerlevel);
		void sendattack(Attack attack);
		void setcontrol(int32_t oid, bool control);

	private:
		void applyattack(const Attack& attack);
		vector<int32_t> findclose(rectangle2d<int16_t> range, uint8_t mobcount) const;
		vector<int32_t> findranged(rectangle2d<int16_t> range, Point<int16_t> origin, uint8_t mobcount) const;
		Optional<Mob> getmob(int32_t oid);

		list<Attack> attacklist;

		class MissBullet
		{
		public:
			MissBullet(Bullet b, Point<int16_t> t)
				: bullet(b), target(t) {}

			void draw(Point<int16_t> viewpos, float alpha) const
			{
				bullet.draw(viewpos, alpha);
			}

			bool update()
			{
				return bullet.update(target);
			}

		private:
			Bullet bullet;
			Point<int16_t> target;
		};
		list<MissBullet> missbullets;
	};
}

