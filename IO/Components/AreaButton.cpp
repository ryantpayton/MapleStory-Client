/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "AreaButton.h"

namespace jrc
{
	AreaButton::AreaButton(Point<int16_t> pos, Point<int16_t> dim)
	{
		position = pos;
		dimension = dim;
		state = NORMAL;
		active = true;
	}

	Rectangle<int16_t> AreaButton::bounds(Point<int16_t> parentpos) const
	{
		Point<int16_t> absp = position + parentpos;
		return Rectangle<int16_t>(absp, absp + dimension);
	}
}
