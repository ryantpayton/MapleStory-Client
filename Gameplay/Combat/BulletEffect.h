#pragma once
#include "Bullet.h"
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
#include "DamageEffect.h"

namespace jrc
{
	class BulletEffect
	{
	public:
		BulletEffect(const Bullet& bullet, Point<int16_t> target, const DamageEffect& damage_effect);

		void draw(double viewx, double viewy, float alpha) const;
		bool update();
		bool update(Point<int16_t> newtarget);

		int32_t get_target() const;
		const DamageEffect& get_effect() const;

	private:
		Bullet bullet;
		Point<int16_t> target;
		DamageEffect damageeffect;
		bool fired;
	};
}