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
#include "Sprite.h"
#include "Texture.h"
#include "Animation.h"

namespace Graphics
{
	Sprite::Sprite(node src, vector2d<int> pos)
	{
		if (src.data_type() == nl::node::type::bitmap)
		{
			graphic = new Texture(src);
		}
		else
		{
			graphic = new Animation(src);
		}

		position = pos;
	}

	Sprite::~Sprite()
	{
		if (graphic != 0)
		{
			delete graphic;
		}
	}

	void Sprite::draw(DrawArgument* args)
	{
		if (graphic != 0)
		{
			graphic->draw(&PosArgument(args->getpos() + position));
		}
	}

	void Sprite::update(short dpf)
	{
		if (graphic != 0)
		{
			graphic->update(dpf);
		}
	}
}
