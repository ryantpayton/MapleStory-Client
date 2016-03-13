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
	Bullet::Bullet(Animation a, Point<int16_t> origin, bool toleft)
	{
		animation = a;
		flip = !toleft;
		moveobj.fx = origin.x() + (toleft ? -30.0f : 30.0f);
		moveobj.fy = origin.y() - 26.0f;
		moveobj.hspeed = 0.0f;
		moveobj.vspeed = 0.0f;
	}

	void Bullet::draw(Point<int16_t> viewpos, float alpha) const
	{
		Point<int16_t> bulletpos = getposition(alpha) + viewpos;
		animation.draw(bulletpos, alpha);
	}

	Point<int16_t> Bullet::getposition(float alpha) const
	{
		return moveobj.getposition(alpha);
	}

	bool Bullet::update(Point<int16_t> target)
	{
		animation.update();

		double xdelta = target.x() - moveobj.fx;
		double ydelta = target.y() - moveobj.fy;
		moveobj.hspeed = (moveobj.hspeed + xdelta / 10) / 2;
		moveobj.vspeed = (moveobj.vspeed + ydelta / 10) / 2;
		flip = xdelta < 0.0;
		moveobj.move();
		return Point<double>(xdelta, ydelta).length() < 10.0;
	}
}