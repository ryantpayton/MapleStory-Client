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
#include "Button.h"
#include "Graphics\Texture.h"
#include <map>

namespace IO
{
	using::std::map;
	using::nl::node;
	using::Graphics::Texture;

	// A standard maplestory-button with 4 states and a texture for each state.
	class MapleButton : public Button
	{
	public:
		MapleButton(node, vector2d<int32_t>);
		void draw(vector2d<int32_t>) const;
		rectangle2d<int32_t> bounds(vector2d<int32_t>) const;
	private:
		map<State, Texture> textures;
	};
}

