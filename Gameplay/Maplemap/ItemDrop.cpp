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
#include "ItemDrop.h"

namespace Gameplay
{
	ItemDrop::ItemDrop(int32_t oid, int32_t owner, Point<int16_t> start,
		Point<int16_t> dest, int8_t type, int8_t mode, Texture icn, 
		int32_t iid, bool pldrp) : Drop(oid, owner, start, dest, type, mode, pldrp) {

		icon = icn;
		itemid = iid;
	}

	void ItemDrop::draw(const Camera& camera, float inter) const
	{
		if (!active)
			return;

		Point<int16_t> absp = phobj.getposition(inter) + camera.getposition(inter);

		using Graphics::DrawArgument;
		icon.draw(DrawArgument(absp, opacity));
	}
}