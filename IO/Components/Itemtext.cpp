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
#include "Itemtext.h"

namespace IO
{
	Itemtext::Itemtext(string text, int16_t maxwidth)
	{
		vector<string> strings;
		size_t pos = 0;
		size_t split = text.find("\\n", pos);
		while (split != string::npos)
		{
			strings.push_back(text.substr(pos, split));
			pos = split + 2;
			split = text.find("\\n", pos);
		}
		strings.push_back(text.substr(pos));

		vector2d<int16_t> offset;
		for (auto& str : strings)
		{
			size_t length = str.size();
			size_t pos2 = 0;

			Text::Font font = Text::A12M;
			Text::Color color = Text::WHITE;

			while (pos2 < length)
			{
				size_t special = str.find_first_of('#', pos2);
				if (special != string::npos)
				{
					if (length > special + 1)
					{
						Text::Color lastc = color;
						char arg = str[special + 1];
						switch (arg)
						{
						case 'c':
							color = Text::ORANGE;
							break;
						}

						if (color != lastc)
						{
							Text linetext = Text(font, Text::LEFT, lastc);
							linetext.settext(str.substr(pos2, special), maxwidth);
							lines.push_back(Line(linetext, offset));
							offset += linetext.endoffset();
						}

						pos2 = special + 2;
					}
					else
					{
						str.erase(special);
					}
				}
				else
				{
					Text linetext = Text(font, Text::LEFT, color);
					linetext.settext(str.substr(pos2), maxwidth);
					lines.push_back(Line(linetext, offset));
					pos2 = length;
					offset = vector2d<int16_t>(0, offset.y() + linetext.height() + 2);
				}
			}
		}

		layout = vector2d<int16_t>(maxwidth, offset.y());
	}

	Itemtext::Itemtext() {}

	Itemtext::~Itemtext() {}

	void Itemtext::draw(vector2d<int16_t> position) const
	{
		for (auto& line : lines)
		{
			vector2d<int16_t> absp = position + line.offset;
			line.text.draw(absp);
		}
	}

	int16_t Itemtext::getheight() const
	{
		return layout.y();
	}
}