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
#include <map>

namespace IO
{
	Text& getlabel(Text::Color color)
	{
		using std::map;
		static map<Text::Color, Text> labels;
		if (!labels.count(color))
			labels[color] = Text(Text::A11M, Text::RIGHT, color);

		return labels[color];
	}

	StatusMessenger::StatusMessenger() {}

	StatusMessenger::~StatusMessenger() {}

	void StatusMessenger::draw(vector2d<int16_t> position, float inter) const
	{
		int16_t offset = static_cast<int16_t>(statusinfos.size()) * 16;
		position.shifty(-offset);

		for (auto& inf : statusinfos)
		{
			Text& label = getlabel(inf.color);
			label.setalpha(inf.getalpha(inter));
			label.drawline(inf.text, position);
			position.shifty(16);
		}
	}

	void StatusMessenger::update()
	{
		int32_t remove = 0;
		for (auto& inf : statusinfos)
		{
			inf.lastalpha = inf.alpha;
			inf.alpha -= 0.00125f;
			if (inf.alpha < 0.00125f)
				remove++;
		}

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