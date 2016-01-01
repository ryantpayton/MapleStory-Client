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
#include "BuffInventory.h"
#include "Data\DataFactory.h"

namespace IO
{
	BuffInventory::BuffInventory() {}

	BuffInventory::~BuffInventory() {}

	void BuffInventory::draw(vector2d<int16_t> position, float) const
	{
		for (auto& icon : icons)
		{
			const Texture* texture = icon.second.texture;
			if (texture)
			{
				using Graphics::DrawArgument;
				texture->draw(DrawArgument(position));
				position.shiftx(-32);
			}
		}
	}

	void BuffInventory::update()
	{

	}

	void BuffInventory::addbuff(int32_t buffid, int32_t duration)
	{
		const Texture* texture = Data::getskill(buffid).geticon(0);
		if (texture)
			icons[buffid] = BuffIcon{ texture, duration };
	}

	void BuffInventory::cancelbuff(int32_t buffid)
	{
		icons.erase(buffid);
	}
}