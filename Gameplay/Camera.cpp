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
#pragma once
#include "Camera.h"
#include "Program\Constants.h"

namespace Gameplay
{
	Camera::Camera()
	{
		fx = 0.0f;
		fy = 0.0f;
		lastx = 0.0f;
		lasty = 0.0f;
	}

	Camera::~Camera() {}

	void Camera::update(vector2d<int16_t> playerpos)
	{
		static const float hspeedf = 12 / Constants::fVIEWWIDTH;
		static const float vspeedf = 12 / Constants::fVIEWHEIGHT;

		float destx = (Constants::fVIEWWIDTH / 2) - static_cast<float>(playerpos.x());
		float desty = (Constants::fVIEWHEIGHT / 2) - static_cast<float>(playerpos.y());

		lastx = fx;
		lasty = fy;

		if (abs(destx - fx) > 1.0f)
			fx += hspeedf * (destx - fx);

		if (abs(desty - fy) > 1.0f)
			fy += vspeedf * (desty - fy);
	}

	void Camera::setposition(vector2d<int16_t> pos)
	{
		fx = (Constants::fVIEWWIDTH / 2) - static_cast<float>(pos.x());
		fy = (Constants::fVIEWHEIGHT / 2) - static_cast<float>(pos.y());
	}

	void Camera::updateview(vector2d<int16_t> mapwalls, vector2d<int16_t> mapborders)
	{
		hbounds = vector2d<int16_t>(-mapwalls.x(), -mapwalls.y());
		vbounds = vector2d<int16_t>(-mapborders.x(), -mapborders.y());
	}

	vector2d<int16_t> Camera::getposition(float inter) const
	{
		int16_t retx = static_cast<int16_t>((1.0f - inter) * lastx + inter * fx);
		int16_t rety = static_cast<int16_t>((1.0f - inter) * lasty + inter * fy);

		if (retx > hbounds.x() || hbounds.length() < Constants::VIEWWIDTH)
			retx = hbounds.x();
		else if (retx < hbounds.y() + Constants::VIEWWIDTH)
			retx = hbounds.y() + Constants::VIEWWIDTH;

		if (rety < vbounds.y() + Constants::VIEWHEIGHT || vbounds.length() < Constants::VIEWHEIGHT)
			rety = vbounds.y() + Constants::VIEWHEIGHT;

		return vector2d<int16_t>(retx, rety);
	}
}
