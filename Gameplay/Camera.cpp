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

#include "..\Constants.h"

namespace jrc
{
	Camera::Camera()
	{
		x.set(0.0);
		y.set(0.0);
	}

	void Camera::update(Point<int16_t> position)
	{
		double nextx = x.get();
		double hdelta = Constants::VIEWWIDTH / 2 - position.x() - nextx;
		if (std::abs(hdelta) >= 5.0)
		{
			nextx += hdelta * (12.0 / Constants::VIEWWIDTH);
		}
		
		double nexty = y.get();
		double vdelta = Constants::VIEWHEIGHT / 2 - position.y() - nexty;
		if (std::abs(vdelta) >= 5.0)
		{
			nexty += vdelta * (12.0 / Constants::VIEWHEIGHT);
		}

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

	Point<int16_t> Camera::position() const
	{
		auto shortx = static_cast<int16_t>(std::round(x.get()));
		auto shorty = static_cast<int16_t>(std::round(y.get()));
		return{ shortx, shorty };
	}

	Point<int16_t> Camera::position(double alpha) const
	{
		auto interx = static_cast<int16_t>(std::round(x.get(alpha)));
		auto intery = static_cast<int16_t>(std::round(y.get(alpha)));
		return{ interx, intery };
	}

	Point<double> Camera::realposition(double alpha) const
	{
		return{ x.get(alpha), y.get(alpha) };
	}
}
