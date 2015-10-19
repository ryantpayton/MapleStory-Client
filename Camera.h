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
#include "vector2d.h"

using namespace Util;

namespace Gameplay
{
	class Camera
	{
	public:
		Camera();
		~Camera() {}
		vector2d<int> update(vector2d<int>, short);
		vector2d<int> getposition();
		void setposition(vector2d<int>);
		void updateview(vector2d<int>, vector2d<int>);
	private:
		float hspeed;
		float vspeed;
		float fx;
		float fy;
		vector2d<int> hbounds;
		vector2d<int> vbounds;
	};
}

