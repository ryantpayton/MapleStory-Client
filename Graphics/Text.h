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
#pragma once
#include "Util\Point.h"
#include <cstdint>
#include <map>
#include <vector>

namespace Graphics
{
	using std::map;
	using std::vector;
	using std::string;

	class Text
	{
	public:
		static const size_t NUM_FONTS = 8;
		enum Font
		{
			A11L,
			A11M,
			A11B,
			A12M,
			A12B,
			A13M,
			A13B,
			A18M
		};

		enum Alignment
		{
			LEFT,
			CENTER,
			RIGHT
		};

		static const size_t NUM_COLORS = 12;
		enum Color
		{
			BLACK,
			WHITE,
			YELLOW,
			BLUE,
			RED,
			DARKRED,
			BROWN,
			LIGHTGREY,
			DARKGREY,
			ORANGE,
			MEDIUMBLUE,
			VIOLET
		};

		enum Background
		{
			NONE,
			NAMETAG
		};

		struct Layout
		{
			struct Line
			{
				Point<int16_t> position;
				size_t first;
				size_t last;
			};

			Alignment alignment;

			Point<int16_t> dimensions;
			Point<int16_t> endoffset;
			map<size_t, float> advances;

			vector<Line> lines;
			int16_t linecount;

			Layout(Alignment a)
			{
				alignment = a;
				linecount = 0;
			}

			Layout()
			{
				alignment = Text::LEFT;
				linecount = 0;
			}

			void addline(int16_t ax, int16_t y, size_t first, size_t last)
			{
				int16_t x = 0;
				switch (alignment)
				{
				case Text::CENTER:
					x -= ax / 2;
					break;
				case Text::RIGHT:
					x -= ax;
					break;
				}
				Point<int16_t> position = Point<int16_t>(x, y);
				Line line = {position, first, last};
				lines.push_back(line);
				linecount++;
			}
		};

		Text(Font font, Alignment alignment, Color color);
		Text();

		void draw(Point<int16_t> position, float alpha) const;
		void draw(Point<int16_t> position) const;

		void settext(const string& text);
		void settext(const string& text, uint16_t maxwidth);
		void setcolor(Color color);
		void setback(Background background);

		size_t length() const;
		int16_t width() const;
		int16_t height() const;
		int16_t linecount() const;
		uint16_t advance(size_t pos) const;
		Point<int16_t> dimensions() const;
		Point<int16_t> endoffset() const;
		string gettext() const;

	private:
		Font font;
		Alignment alignment;
		Color color;
		Background background;
		Layout layout;
		string text;
	};
}