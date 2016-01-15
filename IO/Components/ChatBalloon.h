/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 Daniel Allendorf                                               //
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

namespace IO
{
	using std::string;
	using Util::vector2d;
	using Graphics::Texture;
	using Graphics::Text;

	class ChatBalloon
	{
	public:
		ChatBalloon(int8_t);
		ChatBalloon();
		~ChatBalloon();

		void draw(vector2d<int16_t> position) const;
		void update();
		void settext(string text);
		void expire();

	private:
		Text textlabel;
		Texture arrow;
		Texture center;
		Texture east;
		Texture northeast;
		Texture north;
		Texture northwest;
		Texture west;
		Texture southwest;
		Texture south;
		Texture southeast;
		int16_t xtile;
		int16_t ytile;

		int16_t width;
		int16_t height;
		int16_t duration;
	};
}

