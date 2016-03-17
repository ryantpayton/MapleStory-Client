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
#include "Constants.h"

#include "Graphics\GraphicsGL.h"

namespace IO
{
	using Graphics::GraphicsGL;

	ScrollingNotice::ScrollingNotice()
	{
		notice = Text(Text::A12M, Text::LEFT, Text::YELLOW);
		xpos.set(0.0f);
		active = false;
	}

	void ScrollingNotice::setnotice(string n)
	{
		notice.settext(n);
		xpos.set(800.0f);
		active = n.size() > 0;
	}

	void ScrollingNotice::draw(float alpha) const
	{
		if (active)
		{
			GraphicsGL::get().drawrectangle(0, -Constants::VIEWYOFFSET, 800, 20, 0.0f, 0.0f, 0.0f, 0.5f);
			int16_t interx = static_cast<int16_t>(xpos.get(alpha));
			auto position = Point<int16_t>(interx, -Constants::VIEWYOFFSET - 2);
			notice.draw(position);
		}
	}

	void ScrollingNotice::update()
	{
		if (active)
		{
			xpos -= 0.5f;
			if (xpos < -notice.width())
			{
				xpos.set(800.0f);
			}
		}
	}
}