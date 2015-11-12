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
#include "MapleButton.h"

namespace IO
{
	MapleButton::MapleButton(node src, vector2d<int32_t> pos)
	{
		textures[PRESSED] = Texture(src["pressed"]["0"]);
		textures[MOUSEOVER] = Texture(src["mouseOver"]["0"]);
		textures[NORMAL] = Texture(src["normal"]["0"]);
		textures[DISABLED] = Texture(src["disabled"]["0"]);
		position = pos;
		state = NORMAL;
		active = true;
	}

	MapleButton::MapleButton(node src)
	{
		textures[PRESSED] = Texture(src["pressed"]["0"]);
		textures[MOUSEOVER] = Texture(src["mouseOver"]["0"]);
		textures[NORMAL] = Texture(src["normal"]["0"]);
		textures[DISABLED] = Texture(src["disabled"]["0"]);
		state = NORMAL;
		active = true;
	}

	void MapleButton::draw(vector2d<int32_t> parentpos) const
	{
		if (active)
		{
			using::Graphics::DrawArgument;
			textures.at(state).draw(DrawArgument(position + parentpos));
		}
	}

	rectangle2d<int32_t> MapleButton::bounds(vector2d<int32_t> parentpos) const
	{
		vector2d<int32_t> absp = parentpos + position - textures.at(state).getorigin();
		return rectangle2d<int32_t>(absp, absp + textures.at(state).getdimensions());
	}
}
