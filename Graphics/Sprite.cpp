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
#include "Sprite.h"

namespace Graphics
{
	Sprite::Sprite(Animation a, DrawArgument args)
		: animation(a), stateargs(args) {}

	Sprite::Sprite(node src, DrawArgument args)
		: animation(src), stateargs(args) {}

	Sprite::Sprite(node src)
		: Sprite(src, DrawArgument()) {}

	Sprite::~Sprite() {}

	void Sprite::draw(Point<int16_t> parentpos, float alpha) const
	{
		auto absargs = stateargs + parentpos;
		animation.draw(absargs, alpha);
	}

	bool Sprite::update(uint16_t timestep)
	{
		return animation.update(timestep);
	}

	bool Sprite::update()
	{
		return animation.update();
	}
}
