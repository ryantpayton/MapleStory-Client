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
#include "UIStatusMessenger.h"
#include <numeric>

namespace IO
{
	UIStatusMessenger::UIStatusMessenger()
	{
		position = Point<int16_t>(790, 510);
	}

	void UIStatusMessenger::draw(float inter) const
	{
		int16_t offset = static_cast<int16_t>(statusinfos.size()) * 16;
		Point<int16_t> infopos = Point<int16_t>(position.x(), position.y() - offset);
		for (auto& info : statusinfos)
		{
			info.draw(infopos, inter);
			infopos.shifty(16);
		}
	}

	void UIStatusMessenger::update()
	{
		int32_t remove = std::accumulate(statusinfos.begin(), statusinfos.end(), 0, [](const int32_t& x, StatusInfo& info){
			return info.update() ? (x + 1) : x;
		});

		statusinfos.erase(statusinfos.begin(), statusinfos.begin() + remove);
	}

	void UIStatusMessenger::showstatus(Text::Color color, string message)
	{
		StatusInfo statusinfo = StatusInfo(message, color);
		statusinfos.push_back(statusinfo);
	}
}