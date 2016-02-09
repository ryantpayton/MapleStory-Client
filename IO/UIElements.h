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
#include "Element.h"
#include "UIElement.h"

namespace IO
{
	class UIElements
	{
	public:
		virtual ~UIElements() {}

		virtual void draw(float inter, vector2d<int16_t> cursor) const = 0;
		virtual void update() = 0;
		virtual void doubleclick(vector2d<int16_t> pos) = 0;
		virtual Cursor::Mousestate sendmouse(Cursor::Mousestate mst, vector2d<int16_t> pos) = 0;

		virtual void add(const Element& element) = 0;
		virtual void remove(Element::UIType type) = 0;
		virtual UIElement* get(Element::UIType type) const = 0;
		virtual UIElement* getfront(vector2d<int16_t> pos) const = 0;
	};
}