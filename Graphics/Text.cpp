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
#include "GraphicsEngine.h"

namespace Graphics
{
	Text::Text(Font f, Alignment a, Color c)
	{
		font = f;
		alignment = a;
		color = c;

		text = "";
		background = NONE;
	}

	Text::Text() {}

	void Text::settext(const string& t)
	{
		settext(t, 0);
	}

	void Text::settext(const string& t, uint16_t maxwidth)
	{
		text = string(t);
		if (text.size() > 0)
		{
			if (maxwidth < 1)
				maxwidth = 800;

			layout = GraphicsEngine::get().createlayout(text, font, alignment, maxwidth);
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

	void Text::draw(vector2d<int16_t> pos) const
	{
		draw(pos, 1.0f);
	}

	void Text::draw(vector2d<int16_t> position, float alpha) const
	{
		GraphicsEngine::get().drawtext(text, font, alignment, color, background, layout, alpha, position);
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

	vector2d<int16_t> Text::dimensions() const
	{
		return layout.dimensions;
	}

	vector2d<int16_t> Text::endoffset() const
	{
		return layout.endoffset;
	}

	string Text::gettext() const
	{
		return text;
	}
}