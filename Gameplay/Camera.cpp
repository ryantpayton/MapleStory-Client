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
#include "Program\TimeConstants.h"

namespace Gameplay
{
	const float fVIEWWIDTH = 800.0f;
	const float fVIEWHEIGHT = 600.0f;
	const int32_t VIEWWIDTH = 800;
	const int32_t VIEWHEIGHT = 600;

	Camera::Camera()
	{
		fx = 0.0f;
		fy = 0.0f;
		lastx = 0.0f;
		lasty = 0.0f;
		hspeed = 0.0f;
		vspeed = 0.0f;
	}

	Camera::~Camera() {}

	void Camera::update(vector2d<int32_t> playerpos)
	{
		float destx = (fVIEWWIDTH / 2) - static_cast<float>(playerpos.x());
		float desty = (fVIEWHEIGHT / 2) - static_cast<float>(playerpos.y());

		lastx = fx;
		lasty = fy;

		if (abs(destx - fx) > 1.0f || abs(desty - fy) > 1.0f)
		{
			hspeed = 1.5f * static_cast<float>(Constants::TIMESTEP) * (destx - fx) / fVIEWWIDTH;
			vspeed = 1.5f * static_cast<float>(Constants::TIMESTEP) * (desty - fy) / fVIEWHEIGHT;

			fx += hspeed;
			fy += vspeed;

			if (abs(destx - fx) <= 1.0f)
			{
				hspeed = 0.0f;
			}
			if (abs(desty - fy) <= 1.0f)
			{
				vspeed = 0.0f;
			}
		}
	}

	void Camera::setposition(vector2d<int32_t> pos)
	{
		fx = (fVIEWWIDTH / 2) - static_cast<float>(pos.x());
		fy = (fVIEWHEIGHT / 2) - static_cast<float>(pos.y());
	}

	void Camera::updateview(vector2d<int32_t> mapwalls, vector2d<int32_t> mapborders)
	{
		hbounds = vector2d<int32_t>(-mapwalls.x(), -mapwalls.y());
		vbounds = vector2d<int32_t>(-mapborders.x(), -mapborders.y());
	}

	vector2d<int32_t> Camera::getposition(float inter) const
	{
		int32_t retx = static_cast<int32_t>((1.0f - inter) * lastx + inter * fx);
		int32_t rety = static_cast<int32_t>((1.0f - inter) * lasty + inter * fy);

		if (retx > hbounds.x() || hbounds.length() < VIEWWIDTH)
			retx = hbounds.x();
		else if (retx < hbounds.y() + VIEWWIDTH)
			retx = hbounds.y() + VIEWWIDTH;

		if (rety < vbounds.y() + VIEWHEIGHT || vbounds.length() < VIEWHEIGHT)
			rety = vbounds.y() + VIEWHEIGHT;

		return vector2d<int32_t>(retx, rety);
	}
}
