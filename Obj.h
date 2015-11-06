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
#include "Animation.h"

namespace Gameplay
{
	using::std::int32_t;
	using::Util::vector2d;
	using::Graphics::Animation;
	// Represents an obj (map decoration) on a map. Shares animation with other objs on the same map.
	class Obj
	{
	public:
		// Construct the obj with the specified values.
		// Parameters: Animation&(reference to shared animation), vector2d(position), bool(is flipped)
		Obj(const Animation&, vector2d<int32_t>, bool);
		// Empty destructor.
		~Obj();
		// Draw the obj at the specified position.
		void draw(vector2d<int32_t>) const;
	private:
		const Animation& anim;
		vector2d<int32_t> pos;
		bool flip;
	};
}
