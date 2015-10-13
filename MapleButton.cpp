/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
	MapleButton::MapleButton(node src, vector2d<int> pos)
	{
		textures.add(BTS_PRESSED, new Texture(src["pressed"]["0"]));
		textures.add(BTS_MOUSEOVER , new Texture(src["mouseOver"]["0"]));
		textures.add(BTS_NORMAL, new Texture(src["normal"]["0"]));
		textures.add(BTS_DISABLED, new Texture(src["disabled"]["0"]));
		position = pos;
		state = BTS_NORMAL;
		active = true;
	}

	void MapleButton::draw(vector2d<int> parentpos)
	{
		if (active)
		{
			textures.get(state)->draw(&PosArgument(position + parentpos));
		}
	}

	rectangle2d<int> MapleButton::bounds(vector2d<int> parentpos)
	{
		vector2d<int> absp = parentpos + position - textures.get(state)->getorigin();
		return rectangle2d<int>(absp, absp + textures.get(state)->getdimension());
	}
}
