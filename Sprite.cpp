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
#include "Sprite.h"
#include "Texture.h"
#include "Animation.h"

namespace Graphics
{
	Sprite::Sprite(node src, vector2d<int32_t> pos)
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
		if (graphic)
		{
			delete graphic;
		}
	}

	void Sprite::draw(const DrawArgument& args) const
	{
		if (graphic)
		{
			graphic->draw(PosArgument(args.getpos() + position));
		}
	}

	void Sprite::update(uint16_t dpf)
	{
		if (graphic)
		{
			graphic->update(dpf);
		}
	}

	vector2d<int32_t> Sprite::getorigin() const
	{
		return graphic ? graphic->getorigin() : vector2d<int>();
	}

	vector2d<int32_t> Sprite::getdimensions() const
	{
		return graphic ? graphic->getdimensions() : vector2d<int>();
	}
}
