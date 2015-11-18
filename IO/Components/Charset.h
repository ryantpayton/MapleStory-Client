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
#include "Graphics\Texture.h"
#include <map>

namespace IO
{
	using std::int8_t;
	using std::int16_t;
	using std::string;
	using std::map;
	using nl::node;
	using Graphics::Texture;
	using Graphics::DrawArgument;

	class Charset
	{
	public:
		enum Alignment
		{
			LEFT,
			CENTER,
			RIGHT
		};

		Charset(node, Alignment);
		Charset(){}
		~Charset();
		void draw(int8_t, const DrawArgument&) const;
		int16_t draw(string, const DrawArgument&) const;
		int16_t getw(int8_t) const;
	private:
		Alignment alignment;
		map<int8_t, Texture> chars;
	};
}

