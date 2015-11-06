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
#include "Texture.h"
#include "Ptrmap.h"

namespace IO
{
	enum CharsetAlignment
	{
		CHA_LEFT,
		CHA_CENTER,
		CHA_RIGHT
	};

	using::std::int8_t;
	using::std::int32_t;
	using::std::string;
	using::nl::node;
	using::Util::Ptrmap;
	using::Graphics::Texture;
	using::Graphics::DrawArgument;

	class Charset
	{
	public:
		Charset(node, CharsetAlignment);
		Charset(){}
		~Charset();
		void draw(int8_t, const DrawArgument&) const;
		int32_t draw(string, const DrawArgument&) const;
		int32_t getw(int8_t) const;
	private:
		CharsetAlignment alignment;
		Ptrmap<int8_t, Texture> chars;
	};
}

