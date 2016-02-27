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
#include "Bullet.h"

namespace Gameplay
{
	Bullet::Bullet(Animation a, vector2d<int16_t> origin, bool toleft)
	{
		animation = a;
		fx = origin.x() + (toleft ? -30.0f : 30.0f);
		fy = origin.y() - 26.0f;
		flip = toleft;
		hspeed = 0.0f;
		vspeed = 0.0f;
	}

	void Bullet::draw(vector2d<int16_t> viewpos, float alpha) const
	{
		vector2d<int16_t> bulletpos = getposition(alpha) + viewpos;
		animation.draw(bulletpos, alpha);
	}

	vector2d<int16_t> Bullet::getposition(float alpha) const
	{
		int16_t interx = static_cast<int16_t>((1.0f - alpha) * lastx + alpha * fx);
		int16_t intery = static_cast<int16_t>((1.0f - alpha) * lasty + alpha * fy);
		return vector2d<int16_t>(interx, intery);
	}

	bool Bullet::update(vector2d<int16_t> target)
	{
		animation.update();

		float xdelta = target.x() - fx;
		float ydelta = target.y() - fy;
		hspeed = (hspeed + xdelta / 10) / 2;
		vspeed = (vspeed + ydelta / 10) / 2;
		flip = xdelta > 0.0f;
		lastx = fx;
		lasty = fy;
		fx += hspeed;
		fy += vspeed;
		return vector2d<float>(xdelta, ydelta).length() < 10.0f;
	}
}