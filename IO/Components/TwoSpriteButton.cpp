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
#include "TwoSpriteButton.h"

namespace IO
{
	TwoSpriteButton::TwoSpriteButton(node nrm, node sel, vector2d<int16_t> pos)
	{
		textures[false] = Texture(nrm);
		textures[true] = Texture(sel);
		position = pos;
		state = NORMAL;
		active = true;
	}

	void TwoSpriteButton::draw(vector2d<int16_t> parentpos) const
	{
		if (active)
		{
			bool selected = state == MOUSEOVER || state == PRESSED;

			using Graphics::DrawArgument;
			textures.at(selected).draw(DrawArgument(position + parentpos));
		}
	}

	rectangle2d<int16_t> TwoSpriteButton::bounds(vector2d<int16_t> parentpos) const
	{
		bool selected = state == MOUSEOVER || state == PRESSED;
		vector2d<int16_t> absp = parentpos + position - textures.at(selected).getorigin();
		return rectangle2d<int16_t>(absp, absp + textures.at(selected).getdimensions());
	}
}
