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

namespace Gameplay
{
	const int32_t VIEWWIDTH = 800;
	const int32_t VIEWHEIGHT = 600;

	Camera::Camera()
	{
		posx = 0;
		posy = 0;
		movex = 0.0f;
		movey = 0.0f;
	}

	Camera::~Camera() {}

	void Camera::update(vector2d<int32_t> playerpos, uint16_t dpf)
	{
		vector2d<int32_t> dest = vector2d<int32_t>(VIEWWIDTH / 2, VIEWHEIGHT / 2) - playerpos;
		vector2d<int32_t> pos = vector2d<int32_t>(static_cast<int32_t>(posx), static_cast<int32_t>(posy));

		if ((dest - pos).length() > 1)
		{
			movex = dpf * static_cast<float>(dest.x() - pos.x()) / VIEWWIDTH;
			movey = dpf * static_cast<float>(dest.y() - pos.y()) / VIEWHEIGHT;

			posx = posx + movex;
			posy = posy + movey;

			if (abs(playerpos.x() - posx) < 1)
			{
				posx = static_cast<float>(playerpos.x());
				movex = 0;
			}
			if (abs(playerpos.y() - posy) < 1)
			{
				posy = static_cast<float>(playerpos.y());
				movey = 0;
			}
		}
	}

	void Camera::setposition(vector2d<int32_t> pos)
	{
		posx = static_cast<float>(VIEWWIDTH / 2 - pos.x());
		posy = static_cast<float>(VIEWHEIGHT / 2 - pos.y());
	}

	void Camera::updateview(vector2d<int32_t> mapwalls, vector2d<int32_t> mapborders)
	{
		hbounds = vector2d<int32_t>(-mapwalls.x(), -mapwalls.y());
		vbounds = vector2d<int32_t>(-mapborders.x(), -mapborders.y() - 30);
	}

	vector2d<int32_t> Camera::getposition() const
	{
		int32_t retx = static_cast<int32_t>(posx);
		int32_t rety = static_cast<int32_t>(posy);

		if (retx > hbounds.x() || hbounds.length() < VIEWWIDTH)
			retx = hbounds.x();
		else if (retx < hbounds.y() + VIEWWIDTH)
			retx = hbounds.y() + VIEWWIDTH;

		if (rety < vbounds.y() + VIEWHEIGHT || vbounds.length() < VIEWHEIGHT)
			rety = vbounds.y() + VIEWHEIGHT;

		return vector2d<int32_t>(retx, rety);
	}
}
