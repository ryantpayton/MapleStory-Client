/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
	Camera::Camera()
	{
		fx = 0.0f;
		fy = 0.0f;
		hspeed = 0.0f;
		vspeed = 0.0f;
	}

	vector2d<int> Camera::update(vector2d<int> playerpos, short dpf)
	{
		vector2d<int> pos = vector2d<int>(static_cast<int>(fx), static_cast<int>(fy));
		if ((playerpos - pos).length() > 1)
		{
			hspeed = dpf * static_cast<float>(playerpos.x() - pos.x()) / 800;
			vspeed = dpf * static_cast<float>(playerpos.y() - pos.y()) / 600;

			fx += hspeed;
			fy += vspeed;

			if (abs(playerpos.x() - pos.x()) < 1)
			{
				fx = static_cast<float>(playerpos.x());
				hspeed = 0.0f;
			}
			if (abs(playerpos.y() - pos.y()) < 1)
			{
				fy = static_cast<float>(playerpos.y());
				vspeed = 0.0f;
			}
		}

		return getposition();
	}

	vector2d<int> Camera::getposition()
	{
		int retx = static_cast<int>(fx);
		int rety = static_cast<int>(fy);

		if (retx > hbounds.x() || hbounds.length() < 816)
			retx = hbounds.x();
		else if (retx < hbounds.y() + 816)
			retx = hbounds.y() + 816;

		if (rety < vbounds.y() + 624 || vbounds.length() < 624)
			rety = vbounds.y() + 624;

		return vector2d<int>(retx + 400, rety - 300);
	}

	void Camera::setposition(vector2d<int> pos)
	{
		fx = static_cast<float>(pos.x());
		fy = static_cast<float>(pos.y());
	}

	void Camera::updateview(vector2d<int> walls, vector2d<int> borders)
	{
		hbounds = vector2d<int>(-walls.x(), -walls.y());
		vbounds = vector2d<int>(-borders.x(), -borders.y() - 30);
	}
}
