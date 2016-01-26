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
#include "DynamicText.h"
#include "GraphicsEngine.h"

namespace Graphics
{
	DynamicText::DynamicText(Text::Font df, Text::Alignment da, Text::Color dc, Text::Background db)
	{
		dfont = df;
		dalignment = da;
		dcolor = dc;
		dbackground = db;
	}

	DynamicText::DynamicText(Text::Font df, Text::Alignment da, Text::Color dc)
	{
		dfont = df;
		dalignment = da;
		dcolor = dc;
		dbackground = Text::NONE;
	}

	DynamicText::DynamicText(Text::Font df, Text::Alignment da)
	{
		dfont = df;
		dalignment = da;
		dcolor = Text::BLACK;
		dbackground = Text::NONE;
	}

	DynamicText::DynamicText(Text::Font df)
	{
		dfont = df;
		dalignment = Text::LEFT;
		dcolor = Text::BLACK;
		dbackground = Text::NONE;
	}

	DynamicText::DynamicText()
	{
		dfont = Text::A11M;
		dalignment = Text::LEFT;
		dcolor = Text::BLACK;
		dbackground = Text::NONE;
	}

	void DynamicText::draw(string text, Text::Color color, float opacity, vector2d<int16_t> position) const
	{
		float x = static_cast<float>(position.x());
		float y = static_cast<float>(position.y());

		GraphicsEngine::get().drawtext(
			text, dfont, dalignment, color, dbackground, opacity,
			vector2d<float>(x, y),
			vector2d<float>()
			);
	}

	void DynamicText::draw(string text, Text::Color color, vector2d<int16_t> position) const
	{
		draw(text, color, 1.0f, position);
	}

	void DynamicText::draw(string text, vector2d<int16_t> position) const
	{
		draw(text, dcolor, 1.0f, position);
	}
}