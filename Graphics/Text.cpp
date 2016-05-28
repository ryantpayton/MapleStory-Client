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
#include "Text.h"
#include "GraphicsGL.h"

namespace jrc
{
	Text::Text(Font f, Alignment a, Color c, Background b, const std::string& t)
		: font(f), alignment(a), color(c), background(b) {

		settext(t);
	}

	Text::Text(Font f, Alignment a, Color c, const std::string& t)
		: Text(f, a, c, NONE, t) {}

	Text::Text(Font f, Alignment a, Color c)
		: Text(f, a, c, NONE, "") {}

	Text::Text()
		: Text(A11M, LEFT, BLACK, NONE, "") {}

	void Text::settext(const std::string& t)
	{
		settext(t, 0);
	}

	void Text::settext(const std::string& t, uint16_t maxwidth)
	{
		if (text == t)
			return;

		text = t;

		if (text.size() > 0)
		{
			if (maxwidth < 1)
				maxwidth = 800;

			layout = GraphicsGL::get().createlayout(text, font, alignment, maxwidth);
		}
	}

	void Text::setcolor(Color c)
	{
		color = c;
	}

	void Text::setback(Background b)
	{
		background = b;
	}

	void Text::draw(Point<int16_t> pos) const
	{
		draw(pos, 1.0f);
	}

	void Text::draw(Point<int16_t> position, float opacity) const
	{
		GraphicsGL::get().drawtext(text, layout, font, color, background, position, opacity);
	}

	uint16_t Text::advance(size_t pos) const
	{
		if (layout.advances.count(pos))
		{
			return static_cast<uint16_t>(layout.advances.at(pos));
		}
		else
		{
			return 0;
		}
	}

	size_t Text::length() const
	{
		return text.size();
	}

	int16_t Text::width() const
	{
		return layout.dimensions.x();
	}

	int16_t Text::height() const
	{
		return layout.dimensions.y();
	}

	int16_t Text::linecount() const
	{
		return layout.linecount;
	}

	Point<int16_t> Text::dimensions() const
	{
		return layout.dimensions;
	}

	Point<int16_t> Text::endoffset() const
	{
		return layout.endoffset;
	}

	std::string Text::gettext() const
	{
		return text;
	}
}