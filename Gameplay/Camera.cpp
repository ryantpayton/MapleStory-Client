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
#include "Camera.h"
#include "Constants.h"

namespace Gameplay
{
	Camera::Camera()
	{
		fx = 0.0;
		fy = 0.0;
		lastx = 0.0;
		lasty = 0.0;
	}

	Camera::~Camera() {}

	void Camera::update(Point<int16_t> position)
	{
		static const double hspeed = 12.0 / Constants::VIEWWIDTH;
		static const double vspeed = 12.0 / Constants::VIEWHEIGHT;

		double destx = Constants::VIEWWIDTH / 2 - position.x();
		double desty = Constants::VIEWHEIGHT / 2 - position.y();

		lastx = fx;
		lasty = fy;

		if (abs(destx - fx) > 1.0)
			fx += hspeed * (destx - fx);

		if (abs(desty - fy) > 1.0)
			fy += vspeed * (desty - fy);

		if (fx > hbounds.first() || hbounds.length() < Constants::VIEWWIDTH)
			fx = hbounds.first();
		else if (fx < hbounds.second() + Constants::VIEWWIDTH)
			fx = hbounds.second() + Constants::VIEWWIDTH;

		if (fy > vbounds.first() || vbounds.length() < Constants::VIEWHEIGHT)
			fy = vbounds.first();
		else if (fy < vbounds.second() + Constants::VIEWHEIGHT)
			fy = vbounds.second() + Constants::VIEWHEIGHT;
	}

	void Camera::setposition(Point<int16_t> position)
	{
		fx = Constants::VIEWWIDTH / 2 - position.x();
		fy = Constants::VIEWHEIGHT / 2 - position.y();
	}

	void Camera::updateview(Range<int16_t> mapwalls, Range<int16_t> mapborders)
	{
		hbounds = Range<int16_t>(-mapwalls.first() - 10, -mapwalls.second() + 10);
		vbounds = -mapborders;
	}

	Point<int16_t> Camera::getposition(float inter) const
	{
		int16_t interx = static_cast<int16_t>((1.0f - inter) * lastx + inter * fx);
		int16_t intery = static_cast<int16_t>((1.0f - inter) * lasty + inter * fy);
		return Point<int16_t>(interx, intery);
	}
}
