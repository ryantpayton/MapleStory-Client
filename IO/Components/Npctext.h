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
#include "Graphics\Texture.h"
#include <vector>

namespace IO
{
	using std::vector;
	using std::string;
	using Graphics::Text;
	using Graphics::Texture;

	class Npctext
	{
	public:
		Npctext(string text, int16_t maxwidth);
		Npctext();
		~Npctext();

		void draw(vector2d<int16_t> position) const;

		int16_t getheight() const;

	private:
		struct Line
		{
			Text text;
			vector2d<int16_t> offset;
		};

		struct Image
		{
			Texture texture;
			vector2d<int16_t> offset;
		};

		size_t parseL(string& str, size_t pos);
		size_t parseh(string& str, size_t pos);
		size_t parsez(string& str, size_t pos);
		size_t parsev(string& str, size_t pos);

		vector2d<int16_t> offset;
		vector<Line> lines;
		vector<Image> images;
	};
}