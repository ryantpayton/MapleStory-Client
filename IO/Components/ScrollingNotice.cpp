//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2016 Daniel Allendorf                                        //
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
#include "ScrollingNotice.h"

#include "..\..\Constants.h"

namespace jrc
{
	ScrollingNotice::ScrollingNotice()
	{
		background = ColorBox(800, 20, ColorBox::BLACK, 0.6f);
		backposition = Point<int16_t>(0, -Constants::VIEWYOFFSET);
		notice = Text(Text::A12M, Text::LEFT, Text::YELLOW);
		xpos.set(0.0);
		active = false;
	}

	void ScrollingNotice::setnotice(std::string n)
	{
		notice.settext(n);
		xpos.set(800.0);
		active = n.size() > 0;
	}

	void ScrollingNotice::draw(float alpha) const
	{
		if (active)
		{
			int16_t interx = static_cast<int16_t>(std::round(xpos.get(alpha)));
			auto position = Point<int16_t>(interx, -Constants::VIEWYOFFSET - 2);
			background.draw(backposition);
			notice.draw(position);
		}
	}

	void ScrollingNotice::update()
	{
		if (active)
		{
			xpos -= 0.5;

			auto xmin = static_cast<double>(-notice.width());
			if (xpos.last() < xmin)
			{
				xpos.set(800.0);
			}
		}
	}
}