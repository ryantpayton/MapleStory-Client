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

		moveobj.setx(origin.x() + (toleft ? -30.0 : 30.0));
		moveobj.sety(origin.y() - 26.0);
	}

	void Bullet::draw(Point<int16_t> viewpos, float alpha) const
	{
		using Graphics::DrawArgument;
		Point<int16_t> bulletpos = moveobj.getposition(alpha) + viewpos;
		animation.draw(DrawArgument(bulletpos, flip), alpha);
	}

	Point<int16_t> Bullet::getposition(float alpha) const
	{
		return moveobj.getposition(alpha);
	}

	bool Bullet::update(Point<int16_t> target)
	{
		animation.update();

		double xdelta = target.x() - moveobj.crntx();
		double ydelta = target.y() - moveobj.crnty();
		moveobj.hspeed = (moveobj.hspeed + xdelta / 15) / 2;
		moveobj.vspeed = (moveobj.vspeed + ydelta / 15) / 2;
		flip = xdelta > 0.0;
		moveobj.move();
		return Point<double>(xdelta, ydelta).length() < 10.0;
	}
}