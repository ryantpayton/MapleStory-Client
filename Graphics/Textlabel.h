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

	class Textlabel
	{
	public:
		enum Font
		{
			DWF_TEXTFIELD,
			DWF_12ML,
			DWF_12LL,
			DWF_12MC,
			DWF_12BC,
			DWF_12MR,
			DWF_14ML,
			DWF_14MC,
			DWF_14BC,
			DWF_14MR,
			DWF_20MC
		};

		enum Textcolor
		{
			TXC_BLACK,
			TXC_WHITE,
			TXC_YELLOW,
			TXC_BLUE,
			TXC_RED,
			TXC_BROWN,
			TXC_GREY,
			TXC_ORANGE,
			TXC_MBLUE,
			TXC_VIOLET
		};

		enum Background
		{
			TXB_NONE,
			TXB_NAMETAG,
			TXB_GMCHAT
		};

		Textlabel(Font, Textcolor, string, uint16_t);
		Textlabel();

		void settext(const string&);
		void settext(const string&, uint16_t);
		void setfont(Font);
		void setcolor(Textcolor);
		void setback(Background);
		void setalpha(float);
		void draw(vector2d<int16_t>) const;
		void draw(vector2d<float>) const;
		void drawline(string, vector2d<int16_t>) const;

		uint16_t getadvance(size_t) const;
		size_t getlength() const;
		const string& gettext() const;
		vector2d<float> getdimensions() const;

	private:
		Font font;
		Textcolor color;
		wstring text;
		string str;
		Background back;

		float alpha;
		vector2d<float> dimensions;
		vector2d<float> endpos;
		map<size_t, float> advances;
	};
}