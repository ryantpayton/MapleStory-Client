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
#include "Util\vector2d.h"
#include <cstdint>
#include <map>

namespace Graphics
{
	using std::map;
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

		static const size_t NUM_COLORS = 11;
		enum Color
		{
			BLACK,
			WHITE,
			YELLOW,
			BLUE,
			RED,
			BROWN,
			GREY,
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
			vector2d<float> dimensions;
			vector2d<int16_t> endoffset;
			map<size_t, float> advances;
			int16_t linecount;
		};

		Text(Font font, Alignment alignment, Color color);
		Text();

		void draw(vector2d<int16_t> position) const;
		void draw(vector2d<float> position) const;

		void settext(const string& text);
		void settext(const string& text, uint16_t maxwidth);
		void setfont(Font font);
		void setalignment(Alignment alignment);
		void setcolor(Color color);
		void setback(Background background);
		void setalpha(float alpha);

		size_t length() const;
		int16_t width() const;
		int16_t height() const;
		int16_t linecount() const;
		uint16_t advance(size_t pos) const;
		vector2d<float> dimensions() const;
		vector2d<int16_t> endoffset() const;
		const string& gettext() const;

	private:
		Font font;
		Alignment alignment;
		Color color;
		string text;
		Background background;

		float opacity;
		Layout layout;
	};
}