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
#include "vector2d.h"
#include <cstdint>

namespace Gameplay
{
	using::std::uint16_t;
	using::std::int32_t;
	using::Util::vector2d;
	// View on stage which follows the player object.
	class Camera
	{
	public:
		Camera();
		~Camera();
		void update(vector2d<int32_t>, uint16_t);
		void setposition(vector2d<int32_t>);
		void updateview(vector2d<int32_t>, vector2d<int32_t>);
		vector2d<int32_t> getposition() const;
	private:
		float movex;
		float movey;
		float posx;
		float posy;
		vector2d<int32_t> hbounds;
		vector2d<int32_t> vbounds;
	};
}

