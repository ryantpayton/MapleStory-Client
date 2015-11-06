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
		textures.add(BTS_PRESSED, new Texture(src["pressed"]["0"]));
		textures.add(BTS_MOUSEOVER , new Texture(src["mouseOver"]["0"]));
		textures.add(BTS_NORMAL, new Texture(src["normal"]["0"]));
		textures.add(BTS_DISABLED, new Texture(src["disabled"]["0"]));
		position = pos;
		state = BTS_NORMAL;
		active = true;
	}

	void MapleButton::draw(vector2d<int32_t> parentpos) const
	{
		if (active)
		{
			const Texture* texture = textures.get(state);
			if (texture)
			{
				using::Graphics::PosArgument;
				texture->draw(PosArgument(position + parentpos));
			}
		}
	}

	rectangle2d<int32_t> MapleButton::bounds(vector2d<int32_t> parentpos) const
	{
		const Texture* texture = textures.get(state);
		if (texture)
		{
			vector2d<int32_t> absp = parentpos + position - texture->getorigin();
			return rectangle2d<int32_t>(absp, absp + texture->getdimensions());
		}
		else
		{
			return rectangle2d<int32_t>();
		}
	}
}
