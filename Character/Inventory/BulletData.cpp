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
#include "BulletData.h"

#include <nlnx\nx.hpp>

namespace jrc
{
	BulletData::BulletData(int32_t id) 
		: ItemData(id) {

		nl::node src = src = nl::nx::item["Consume"]["0" + std::to_string(id / 10000) + ".img"]["0" + std::to_string(id)];

		bullet = src["bullet"];
		watk = src["info"]["incPAD"];
	}

	BulletData::BulletData()
	{
		watk = 0;
	}

	int16_t BulletData::get_watk() const
	{
		return watk;
	}

	Animation BulletData::get_animation() const
	{
		return bullet;
	}
}