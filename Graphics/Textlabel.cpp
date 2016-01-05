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
#include "Graphics\Textlabel.h"

#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#else
#include "Graphics\GraphicsD2D.h"
#endif

namespace Graphics
{
	Textlabel::Textlabel(Font f, Textcolor c, string s, uint16_t m)
	{
		font = f;
		color = c;
		settext(s, m);

		alpha = 1.0f;
		back = TXB_NONE;
	}

	Textlabel::Textlabel() {}

	void Textlabel::settext(const string& t)
	{
		settext(t, 0);
	}

	void Textlabel::settext(const string& t, uint16_t wmax)
	{
		str = string(t);
		text = wstring(t.begin(), t.end());

		float fwmax = static_cast<float>(wmax);

#ifdef JOURNEY_USE_OPENGL
#else
		dimensions = GraphicsD2D::createlayout(text, font, fwmax, advances);
#endif
	}

	void Textlabel::setfont(Font f)
	{
		font = f;
	}

	void Textlabel::setcolor(Textcolor c)
	{
		color = c;
	}

	void Textlabel::setback(Background b)
	{
		back = b;
	}

	void Textlabel::setalpha(float a)
	{
		alpha = a;
	}

	void Textlabel::draw(vector2d<int16_t> pos) const
	{
		draw(vector2d<float>(static_cast<float>(pos.x()), static_cast<float>(pos.y())));
	}

	void Textlabel::drawline(string todraw, vector2d<int16_t> pos) const
	{
#ifdef JOURNEY_USE_OPENGL
#else
		GraphicsD2D::drawtext(
			wstring(todraw.begin(), todraw.end()), 
			font, color, back, alpha,
			vector2d<float>(static_cast<float>(pos.x()), static_cast<float>(pos.y())), 
			vector2d<float>()
			);
#endif
	}
	
	void Textlabel::draw(vector2d<float> pos) const
	{
#ifdef JOURNEY_USE_OPENGL
#else
		GraphicsD2D::drawtext(text, font, color, back, alpha, pos, dimensions);
#endif
	}

	uint16_t Textlabel::getadvance(size_t pos) const
	{
		if (advances.count(pos))
		{
			return static_cast<uint16_t>(advances.at(pos));
		}
		else
		{
			return 0;
		}
	}

	size_t Textlabel::getlength() const
	{
		return str.size();
	}

	const string& Textlabel::gettext() const
	{
		return str;
	}

	vector2d<float> Textlabel::getdimensions() const
	{
		return dimensions;
	}
}