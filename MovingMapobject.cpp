//////////////////////////////////////////////////////////////////////////////
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
#include "MovingMapobject.h"

namespace Gameplay
{
	MovingMapobject::MovingMapobject()
	{
		fx = 0.0f;
		fy = 0.0f;
		hspeed = 0.0f;
		vspeed = 0.0f;
	}

	void MovingMapobject::setposition(vector2d<int> pos)
	{
		fx = static_cast<float>(pos.x());
		fy = static_cast<float>(pos.x());
	}

	vector2d<int> MovingMapobject::getposition()
	{
		return vector2d<int>(static_cast<int>(fx), static_cast<int>(fy));
	}
}
