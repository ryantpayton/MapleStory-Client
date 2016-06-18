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
#include "BulletEffect.h"

namespace jrc
{
	BulletEffect::BulletEffect(const Bullet& b, Point<int16_t> t, const DamageEffect& de)
		: bullet(b), target(t), damageeffect(de) {

		fired = false;
	}

	void BulletEffect::draw(double viewx, double viewy, float alpha) const
	{
		if (fired)
		{
			bullet.draw(viewx, viewy, alpha);
		}
	}

	bool BulletEffect::update()
	{
		if (fired)
		{
			return bullet.update(target);
		}
		else
		{
			bool expired = damageeffect.update();
			if (expired)
			{
				fired = true;
				return bullet.settarget(target);
			}
			else
			{
				return false;
			}
		}
	}

	bool BulletEffect::update(Point<int16_t> newtarget)
	{
		target = newtarget;

		return update();
	}

	int32_t BulletEffect::get_target() const
	{
		return damageeffect.get_target();
	}

	const DamageEffect& BulletEffect::get_effect() const
	{
		return damageeffect;
	}
}