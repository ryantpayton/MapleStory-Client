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
#include "Graphics\Texture.h"
#include "IO\Element.h"

namespace IO
{
	using Util::vector2d;
	using Graphics::Texture;

	class Icon
	{
	public:
		Icon(Texture texture, Element::UIType parent, int16_t identifier, int16_t count);
		Icon();
		~Icon();

		void draw(vector2d<int16_t> position) const;
		void dragdraw(vector2d<int16_t> cursorpos) const;

		void startdrag(vector2d<int16_t> offset);
		void resetdrag();
		void setcount(int16_t count);

		int16_t getidentifier() const;
		Element::UIType getparent() const;

	private:
		Texture texture;
		Element::UIType parent; 
		int16_t identifier;
		int16_t count;

		bool dragged;
		vector2d<int16_t> cursoroffset;
	};
}