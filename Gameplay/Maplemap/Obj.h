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
#include "Graphics\Animation.h"
#include "Util\vector2d.h"

namespace Gameplay
{
	using::std::int32_t;
	using::nl::node;
	using::Util::vector2d;
	using::Graphics::Animation;

	// Represents an obj (map decoration) on a map.
	class Obj
	{
	public:
		Obj(node);
		// Empty destructor.
		~Obj();
		void update();
		// Draw the obj at the specified position.
		void draw(vector2d<int32_t>, float) const;
		// Return depth of the obj.
		uint8_t getz() const;

	private:
		Animation animation;
		vector2d<int32_t> pos;
		uint8_t z;
		bool flip;
	};
}
