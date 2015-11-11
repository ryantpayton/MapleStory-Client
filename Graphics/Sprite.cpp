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
#include "Graphics\Sprite.h"

namespace Graphics
{
	Sprite::Sprite(node src, vector2d<int32_t> pos)
	{
		animation = Animation(src);
		position = pos;
	}

	Sprite::~Sprite() {}

	void Sprite::draw(const DrawArgument& args, float inter) const
	{
		animation.draw(DrawArgument(args.getpos() + position), inter);
	}

	void Sprite::update()
	{
		animation.update();
	}

	vector2d<int32_t> Sprite::getorigin() const
	{
		return animation.getorigin();
	}

	vector2d<int32_t> Sprite::getdimensions() const
	{
		return animation.getorigin();
	}
}
