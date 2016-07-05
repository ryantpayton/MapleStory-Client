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
#include "ChatBalloon.h"

#include "../../Constants.h"

#include "nlnx/nx.hpp"
#include "nlnx/node.hpp"

namespace jrc
{
	ChatBalloon::ChatBalloon(int8_t type)
	{
		std::string typestr;
		if (type < 0)
		{
			switch (type)
			{
			case -1:
				typestr = "dead";
				break;
			}
		}
		else
		{
			typestr = std::to_string(type);
		}

		nl::node src = nl::nx::ui["ChatBalloon.img"][typestr];

		arrow = src["arrow"];
		frame = src;

		textlabel = { Text::A11M, Text::CENTER, Text::BLACK, "", 80 };

		duration = 0;
	}

	ChatBalloon::ChatBalloon() 
		: ChatBalloon(0) {}

	void ChatBalloon::change_text(const std::string& text)
	{
		textlabel.change_text(text);

		duration = DURATION;
	}

	void ChatBalloon::draw(Point<int16_t> position) const
	{
		if (duration == 0)
			return;

		int16_t width = textlabel.width();
		int16_t height = textlabel.height();

		frame.draw(position, width, height);
		arrow.draw(position);
		textlabel.draw(position - Point<int16_t>(0, height + 4));
	}

	void ChatBalloon::update()
	{
		duration -= Constants::TIMESTEP;
		if (duration < 0)
			duration = 0;
	}

	void ChatBalloon::expire()
	{
		duration = 0;
	}
}