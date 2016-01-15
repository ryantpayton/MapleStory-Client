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
#include "Graphics\Text.h"

#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#include "Graphics\GraphicsGL.h"
#else
#include "Graphics\GraphicsD2D.h"
#endif

namespace Graphics
{
	Text::Text(Font f, Alignment a, Color c)
	{
		font = f;
		alignment = a;
		color = c;

		str = "";
		text = L"";
		alpha = 1.0f;
		back = NONE;
		textid = 0;
	}

	Text::Text() {}

	void Text::settext(const string& t)
	{
		settext(t, 0);
	}

	void Text::settext(const string& t, uint16_t wmax)
	{
		str = string(t);
		text = wstring(t.begin(), t.end());

		float fwmax = static_cast<float>(wmax);

#ifdef JOURNEY_USE_OPENGL
		layout = GraphicsGL::createlayout(str.c_str(), font, fwmax);
#else
		layout = GraphicsD2D::createlayout(text, font, fwmax);
#endif
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
		back = b;
	}

	void Text::setalpha(float a)
	{
		alpha = a;
	}

	void Text::draw(vector2d<int16_t> pos) const
	{
		draw(vector2d<float>(static_cast<float>(pos.x()), static_cast<float>(pos.y())));
	}

	void Text::drawline(string todraw, vector2d<int16_t> pos) const
	{
#ifdef JOURNEY_USE_OPENGL
		GraphicsGL::drawtext(todraw.c_str(), font, alignment, color, back, alpha,
			vector2d<float>(static_cast<float>(pos.x()), static_cast<float>(pos.y())), 
			vector2d<float>()
			);
#else
		GraphicsD2D::drawtext(
			wstring(todraw.begin(), todraw.end()), 
			font, alignment, color, back, alpha,
			vector2d<float>(static_cast<float>(pos.x()), static_cast<float>(pos.y())), 
			vector2d<float>()
			);
#endif
	}
	
	void Text::draw(vector2d<float> pos) const
	{
#ifdef JOURNEY_USE_OPENGL
		GraphicsGL::drawtext(str.c_str(), font, alignment, color, back, alpha, pos, layout.dimensions);
#else
		GraphicsD2D::drawtext(text, font, alignment, color, back, alpha, pos, layout.dimensions);
#endif
	}

	uint16_t Text::getadvance(size_t pos) const
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

	size_t Text::getlength() const
	{
		return str.size();
	}

	int16_t Text::getwidth() const
	{
		return static_cast<int16_t>(layout.dimensions.x());
	}

	int16_t Text::getheight() const
	{
		return static_cast<int16_t>(layout.dimensions.y());
	}

	vector2d<float> Text::getdimensions() const
	{
		return layout.dimensions;
	}

	vector2d<int16_t> Text::getendoffset() const
	{
		return layout.endoffset;
	}

	const string& Text::gettext() const
	{
		return str;
	}
}