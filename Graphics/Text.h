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
	using std::uint16_t;
	using std::int16_t;
	using std::map;
	using std::wstring;
	using std::string;
	using Util::vector2d;

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
		};

		Text(Font font, Alignment alignment, Color color);
		Text();

		void draw(vector2d<int16_t> position) const;
		void draw(vector2d<float> position) const;
		void drawline(string line, vector2d<int16_t> position) const;

		void settext(const string& text);
		void settext(const string& text, uint16_t maxwidth);
		void setfont(Font font);
		void setalignment(Alignment alignment);
		void setcolor(Color color);
		void setback(Background background);
		void setalpha(float alpha);

		size_t getlength() const;
		int16_t getwidth() const;
		int16_t getheight() const;
		uint16_t getadvance(size_t pos) const;
		vector2d<float> getdimensions() const;
		vector2d<int16_t> getendoffset() const;
		const string& gettext() const;

	private:
		Font font;
		Alignment alignment;
		Color color;
		wstring text;
		string str;
		Background back;

		uint16_t textid;
		float alpha;
		Layout layout;
	};
}