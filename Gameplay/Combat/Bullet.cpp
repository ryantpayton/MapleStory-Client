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

		moveobj.setx(origin.x() + (toleft ? -30.0 : 30.0));
		moveobj.sety(origin.y() - 26.0);
	}

	void Bullet::draw(Point<int16_t> viewpos, float alpha) const
	{
		using Graphics::DrawArgument;
		Point<int16_t> bulletpos = moveobj.getposition(alpha) + viewpos;
		animation.draw(DrawArgument(bulletpos, flip), alpha);
	}

	bool Bullet::settarget(Point<int16_t> target)
	{
		double xdelta = target.x() - moveobj.crntx();
		double ydelta = target.y() - moveobj.crnty();
		if (std::abs(xdelta) < 10.0)
			return true;

		flip = xdelta > 0.0;

		moveobj.hspeed = xdelta / 32;
		if (xdelta > 0.0)
		{
			if (moveobj.hspeed < 3.0)
			{
				moveobj.hspeed = 3.0;
			}
			else if (moveobj.hspeed > 6.0)
			{
				moveobj.hspeed = 6.0;
			}
		}
		else if (xdelta < 0.0)
		{
			if (moveobj.hspeed > -3.0)
			{
				moveobj.hspeed = -3.0;
			}
			else if (moveobj.hspeed < -6.0)
			{
				moveobj.hspeed = -6.0;
			}
		}
		moveobj.vspeed = moveobj.hspeed * ydelta / xdelta;
		return false;
	}

	bool Bullet::update(Point<int16_t> target)
	{
		animation.update();
		moveobj.move();

		int16_t xdelta = target.x() - moveobj.getx();
		return moveobj.hspeed > 0.0 ? xdelta < 10 : xdelta > 10;
	}
}