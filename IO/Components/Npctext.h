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
#include "..\..\Graphics\Text.h"
#include "..\..\Graphics\Texture.h"

#include <vector>

namespace jrc
{
	class Npctext
	{
	public:
		Npctext(std::string text, int16_t maxwidth);
		Npctext();

		void draw(Point<int16_t> position) const;

		int16_t getheight() const;

	private:
		struct Line
		{
			Text text;
			Point<int16_t> offset;
		};

		struct Image
		{
			Texture texture;
			Point<int16_t> offset;
		};

		size_t parseL(std::string& str, size_t pos);
		size_t parseh(std::string& str, size_t pos);
		size_t parsez(std::string& str, size_t pos);
		size_t parsev(std::string& str, size_t pos);

		Point<int16_t> offset;
		std::vector<Line> lines;
		std::vector<Image> images;
	};
}