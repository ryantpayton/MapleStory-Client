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
#include "Graphics\Text.h"
#include <vector>

namespace IO
{
	using std::vector;
	using std::string;
	using Util::vector2d;
	using Graphics::Text;

	class Itemtext
	{
	public:
		Itemtext(string text, int16_t maxwidth);
		Itemtext();
		~Itemtext();

		void draw(vector2d<int16_t> position) const;

		int16_t getheight() const;

	private:
		struct Line
		{
			Text text;
			vector2d<int16_t> offset;

			Line(Text t, vector2d<int16_t> o)
			{
				text = t;
				offset = o;
			}
		};

		vector<Line> lines;
		vector2d<int16_t> layout;
	};
}