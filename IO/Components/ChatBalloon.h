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
#include "MapleFrame.h"

#include "..\..\Graphics\Text.h"

namespace jrc
{
	class ChatBalloon
	{
	public:
		ChatBalloon(int8_t type);
		ChatBalloon();

		void draw(Point<int16_t> position) const;
		void update();

		void change_text(const std::string& text);
		void expire();

	private:
		// How long a line stays on screen. 4 seconds.
		static constexpr int16_t DURATION = 4000;

		MapleFrame frame;
		Text textlabel;
		Texture arrow;
		int16_t duration;
	};
}

