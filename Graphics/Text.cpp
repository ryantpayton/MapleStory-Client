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
		opacity = 1.0f;
		background = NONE;
	}

	Text::Text() {}

	void Text::settext(const string& t)
	{
		settext(t, 0);
	}

	void Text::settext(const string& t, uint16_t wmax)
	{
		text = string(t);

		float fwmax = static_cast<float>(wmax);
		layout = GraphicsEngine::get().createlayout(text, font, fwmax);
	}

	void Text::setfont(Font f)
	{
		font = f;
	}

	void Text::setcolor(Color c)
	{
		color = c;
	}

	void Text::setback(Background b)
	{
		background = b;
	}

	void Text::setalpha(float opc)
	{
		opacity = opc;
	}

	void Text::draw(vector2d<int16_t> pos) const
	{
		draw(vector2d<float>(static_cast<float>(pos.x()), static_cast<float>(pos.y())));
	}
	
	void Text::draw(vector2d<float> pos) const
	{
		GraphicsEngine::get().drawtext(text, font, alignment, color, background, opacity, pos, layout.dimensions);
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
		return static_cast<int16_t>(layout.dimensions.x());
	}

	int16_t Text::height() const
	{
		return static_cast<int16_t>(layout.dimensions.y());
	}

	int16_t Text::linecount() const
	{
		return layout.linecount;
	}

	vector2d<float> Text::dimensions() const
	{
		return layout.dimensions;
	}

	vector2d<int16_t> Text::endoffset() const
	{
		return layout.endoffset;
	}

	const string& Text::gettext() const
	{
		return text;
	}
}