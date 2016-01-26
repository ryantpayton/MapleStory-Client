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
#include "StatusMessenger.h"
#include <numeric>

namespace IO
{
	StatusMessenger::StatusMessenger()
	{
		statustext = DynamicText(Text::A12M, Text::RIGHT);
	}

	void StatusMessenger::draw(vector2d<int16_t> position, float inter) const
	{
		int16_t offset = static_cast<int16_t>(statusinfos.size()) * 16;
		position.shifty(-offset);

		for (auto& inf : statusinfos)
		{
			statustext.draw(inf.text, inf.color, inf.getalpha(inter), position);
			position.shifty(16);
		}
	}

	void StatusMessenger::update()
	{
		int32_t remove = std::accumulate(statusinfos.begin(), statusinfos.end(), 0, [](const int32_t& x, StatusInfo& info){
			return info.update() ? (x + 1) : x;
		});

		statusinfos.erase(statusinfos.begin(), statusinfos.begin() + remove);
	}

	void StatusMessenger::showstatus(Text::Color color, string message)
	{
		StatusInfo sinf;
		sinf.color = color;
		sinf.text = message;
		statusinfos.push_back(sinf);
	}
}