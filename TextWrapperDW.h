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
#include "TextWrapper.h"
#include "LocatorD2D.h"
#include <map>

namespace Graphics
{
	class TextWrapperDW : public TextWrapper
	{
	public:
		TextWrapperDW(Font, Textcolor, string, short);
		~TextWrapperDW();
		void draw(vector2d<int>);
		void settext(string, short);
		void setalpha(float a) { alpha = a; }
		void setback(TextBackground b) { back = b; }
		short getadvance(size_t);
		vector2d<short> getdimensions() { return vector2d<short>(width, height); }
	private:
		int createlayout(short);
		IDWriteTextFormat* font;
		Textcolor color;
		wstring text;
		TextBackground back;
		ID2D1SolidColorBrush* brush;
		ID2D1SolidColorBrush* backbrush;
		float alpha;
		short width;
		short height;
		vector2d<short> endpos;
		map<size_t, float> advances;
	};
}

