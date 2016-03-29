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
		x.set(0.0);
		y.set(0.0);
	}

	void Camera::update(Point<int16_t> position)
	{
		double hdelta = Constants::VIEWWIDTH / 2 - position.x() - x.get();
		if (std::abs(hdelta) < 1.0)
			hdelta = 0.0;

		double vdelta = Constants::VIEWHEIGHT / 2 - position.y() - y.get();
		if (std::abs(vdelta) < 1.0)
			vdelta = 0.0;

		double nextx = x + hdelta * (12.0 / Constants::VIEWWIDTH);
		double nexty = y + vdelta * (12.0 / Constants::VIEWHEIGHT);

		if (nextx > hbounds.first() || hbounds.length() < Constants::VIEWWIDTH)
		{
			nextx = hbounds.first();
		}
		else if (nextx < hbounds.second() + Constants::VIEWWIDTH)
		{
			nextx = hbounds.second() + Constants::VIEWWIDTH;
		}

		if (nexty > vbounds.first() || vbounds.length() < Constants::VIEWHEIGHT)
		{
			nexty = vbounds.first();
		}
		else if (nexty < vbounds.second() + Constants::VIEWHEIGHT)
		{
			nexty = vbounds.second() + Constants::VIEWHEIGHT;
		}

		x = nextx;
		y = nexty;
	}

	void Camera::setposition(Point<int16_t> position)
	{
		x.set(Constants::VIEWWIDTH / 2 - position.x());
		y.set(Constants::VIEWHEIGHT / 2 - position.y());
	}

	void Camera::updateview(Range<int16_t> mapwalls, Range<int16_t> mapborders)
	{
		hbounds = -mapwalls;
		vbounds = -mapborders;
	}

	Point<int16_t> Camera::getposition(float alpha) const
	{
		auto interx = static_cast<int16_t>(x.get(alpha));
		auto intery = static_cast<int16_t>(y.get(alpha));
		return Point<int16_t>(interx, intery);
	}
}
