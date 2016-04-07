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
#include "nlnx\node.hpp"

namespace Graphics
{
	using::nl::node;

	// Combines an Animation with additional state.
	class Sprite
	{
	public:
		Sprite(Animation animation, DrawArgument stateargs);
		Sprite(node src, DrawArgument stateargs);
		Sprite(node src);
		~Sprite();

		void draw(Point<int16_t> parentpos, float alpha) const;
		bool update(uint16_t timestep);
		bool update();

	private:
		Animation animation;
		DrawArgument stateargs;
	};
}


