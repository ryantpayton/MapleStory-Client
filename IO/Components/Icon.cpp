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
#include "Icon.h"

namespace IO
{
	Icon::Icon(Texture t, Element::UIType p, int16_t i, int16_t c = 1)
	{
		texture = t;
		parent = p;
		identifier = i;
		count = c;

		texture.shift(vector2d<int16_t>(0, 32));
		dragged = false;
	}

	Icon::Icon() 
	{
		parent = Element::NONE;
		identifier = 0;
		dragged = false;
	}

	Icon::~Icon() {}

	void Icon::draw(vector2d<int16_t> position) const
	{
		using Graphics::DrawArgument;
		texture.draw(DrawArgument(position, dragged ? 0.5f : 1.0f));
	}

	void Icon::dragdraw(vector2d<int16_t> cursorpos) const
	{
		if (dragged)
		{
			using Graphics::DrawArgument;
			texture.draw(DrawArgument(cursorpos - cursoroffset, 0.5f));
		}
	}

	void Icon::startdrag(vector2d<int16_t> offset)
	{
		cursoroffset = offset;
		dragged = true;
	}

	void Icon::resetdrag()
	{
		dragged = false;
	}

	void Icon::setcount(int16_t c)
	{
		count = c;
	}

	int16_t Icon::getidentifier() const
	{
		return identifier;
	}

	Element::UIType Icon::getparent() const
	{
		return parent;
	}
}