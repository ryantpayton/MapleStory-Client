//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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

#include "Text.h"

namespace jrc
{
	struct OutlinedText
	{
		Text inner;
		Text l;
		Text r;
		Text t;
		Text b;

		OutlinedText(Text::Font font, Text::Alignment alignment, Text::Color innerColor, Text::Color outerColor)
		{
			inner = Text(font, alignment, innerColor);
			l = Text(font, alignment, outerColor);
			r = Text(font, alignment, outerColor);
			t = Text(font, alignment, outerColor);
			b = Text(font, alignment, outerColor);
		}

		OutlinedText() {}

		void draw(Point<int16_t> parentpos) const
		{
			l.draw(parentpos + Point<int16_t>(-1, 0));
			r.draw(parentpos + Point<int16_t>(1, 0));
			t.draw(parentpos + Point<int16_t>(0, -1));
			b.draw(parentpos + Point<int16_t>(0, 1));
			inner.draw(parentpos);
		}

		void change_text(const std::string& text)
		{
			inner.change_text(text);
			l.change_text(text);
			r.change_text(text);
			t.change_text(text);
			b.change_text(text);
		}
	};
}