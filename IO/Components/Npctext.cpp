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
#include "Npctext.h"
#include "Data\DataFactory.h"
#include "Gameplay\Stage.h"

namespace IO
{
	using Data::DataFactory;

	Npctext::Npctext(string text, int16_t maxwidth)
	{
		vector<string> strings;
		size_t pos = 0;
		size_t split = text.find_first_of("\n", pos);
		while (split != string::npos)
		{
			strings.push_back(text.substr(pos, split - pos));
			pos = split + 1;
			split = text.find_first_of("\n", pos);
		}
		strings.push_back(text.substr(pos));

		Text::Font font = Text::A12M;
		Text::Color color = Text::LIGHTGREY;
		for (auto& str : strings)
		{
			size_t length = str.size();
			size_t pos2 = 0;

			while (pos2 < length)
			{
				size_t special = str.find_first_of('#', pos2);
				if (special != string::npos)
				{
					if (special > pos2)
					{
						Text linetext = Text(font, Text::LEFT, color);
						linetext.settext(str.substr(pos2, special - pos2), maxwidth - offset.x());
						lines.push_back({ linetext, offset });
						if (linetext.linecount() > 1) 
							offset.setx(linetext.endoffset().x());
						else
							offset.shiftx(linetext.endoffset().x());
						offset.shifty((linetext.linecount() - 1) * 15);
					}

					if (length > special + 1)
					{
						int8_t arg = str[special + 1];
						switch (arg)
						{
						case 'n':
							font = Text::A12M;
							pos2 = special + 2;
							break;
						case 'e':
							font = Text::A12B;
							pos2 = special + 2;
							break;
						case 'k':
							color = Text::LIGHTGREY;
							pos2 = special + 2;
							break;
						case 'b':
							color = Text::BLUE;
							pos2 = special + 2;
							break;
						case 'r':
							color = Text::RED;
							pos2 = special + 2;
							break;
						case 'z':
							pos = parsez(str, special);
							break;
						case 'v':
							pos = parsev(str, special);
							break;
						case 'h':
							pos = parseh(str, special);
							break;
						case 'L':
							pos = parseL(str, special);
							break;
						default:
							pos2 = special + 2;
						}
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
					lines.push_back({ linetext, offset });
					pos2 = length;
					offset.setx(0);
					offset.shifty(13 + 2);
				}
			}
		}
	}

	Npctext::Npctext() {}

	Npctext::~Npctext() {}

	size_t Npctext::parseL(string& str, size_t pos)
	{
		size_t next = str.find_first_of('#', pos + 2);
		if (next != string::npos)
		{
			try
			{
				//int32_t selection = std::stoi(str.substr(pos + 2, next - pos + 2)); 
				str.replace(str.begin() + pos, str.begin() + next + 1, "");
				return pos + 1;
			}
			catch (const std::exception&)
			{
				str.replace(str.begin() + pos, str.begin() + next + 1, "FORMATERROR");
				return pos + 11 + 1;
			}
		}
		else
		{
			str.replace(str.begin() + pos, str.begin() + pos + 2, "SYNTAXERROR");
			return pos + 11 + 1;
		}
	}

	size_t Npctext::parsez(string& str, size_t pos)
	{
		size_t next = str.find_first_of('#', pos + 2);
		if (next != string::npos)
		{
			try
			{
				using Character::ItemData;
				const ItemData& idata = DataFactory::get().getitemdata(std::stoi(str.substr(pos + 2, next)));
				if (idata.isloaded())
				{
					str.replace(str.begin() + pos, str.begin() + next + 1, idata.getname());
					return pos + idata.getname().length() + 1;
				}
				else
				{
					str.replace(str.begin() + pos, str.begin() + next + 1, "IDERROR");
					return pos + 7 + 1;
				}
			}
			catch (const std::exception&)
			{
				str.replace(str.begin() + pos, str.begin() + next + 1, "FORMATERROR");
				return pos + 11 + 1;
			}
		}
		else
		{
			str.replace(str.begin() + pos, str.begin() + pos + 2, "SYNTAXERROR");
			return pos + 11 + 1;
		}
	}

	size_t Npctext::parseh(string& str, size_t pos)
	{
		size_t next = str.find_first_of('#', pos + 2);
		if (next != string::npos)
		{
			try 
			{
				int32_t type = std::stoi(str.substr(pos + 2, next - pos + 2));
				string insert;
				switch (type)
				{
				case 0:
					using Gameplay::Stage;
					insert = Stage::get().getplayer().getstats().getname();
					break;
				default:
					insert = "";
				}
				str.replace(str.begin() + pos, str.begin() + next + 1, insert);
				return pos + insert.length() + 1;
			}
			catch (const std::exception&) 
			{
				str.replace(str.begin() + pos, str.begin() + next + 1, "FORMATERROR");
				return pos + 11 + 1;
			}
		}
		else
		{
			str.replace(str.begin() + pos, str.begin() + pos + 2, "SYNTAXERROR");
			return pos + 11 + 1;
		}
	}

	size_t Npctext::parsev(string& str, size_t pos)
	{
		size_t next = str.find_first_of('#', pos + 2);
		if (next != string::npos)
		{
			try
			{
				using Character::ItemData;
				const ItemData& idata = DataFactory::get().getitemdata(std::stoi(str.substr(pos + 2, next)));
				if (idata.isloaded())
				{
					Image image = { idata.geticon(false), offset };
					image.texture.shift(Point<int16_t>(0, 32));
					images.push_back(image);
					offset.shiftx(32);
					str.replace(str.begin() + pos, str.begin() + next + 1, "");
					return pos + 1;
				}
				else
				{
					str.replace(str.begin() + pos, str.begin() + next + 1, "IDERROR");
					return pos + 7 + 1;
				}
			}
			catch (const std::exception&)
			{
				str.replace(str.begin() + pos, str.begin() + next + 1, "FORMATERROR");
				return pos + 11 + 1;
			}
		}
		else
		{
			str.replace(str.begin() + pos, str.begin() + pos + 2, "SYNTAXERROR");
			return pos + 11 + 1;
		}
	}

	void Npctext::draw(Point<int16_t> position) const
	{
		for (auto& line : lines)
		{
			line.text.draw(line.offset + position);
		}
		for (auto& image : images)
		{
			image.texture.draw(image.offset + position);
		}
	}

	int16_t Npctext::getheight() const
	{
		return offset.y();
	}
}