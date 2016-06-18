//////////////////////////////////////////////////////////////////////////////
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
#include "MapChars.h"

namespace jrc
{
	void MapChars::draw(int8_t layer, double viewx, double viewy, float alpha) const
	{
		chars.draw(layer, viewx, viewy, alpha);
	}

	void MapChars::update(const Physics& physics)
	{
		chars.update(physics);
	}

	void MapChars::send_spawn(const CharSpawn& spawn)
	{
		int32_t cid = spawn.getcid();
		Optional<OtherChar> ochar = get_char(cid);
		if (ochar)
		{

		}
		else
		{
			chars.add(
				spawn.instantiate()
			);
		}
	}

	void MapChars::remove_char(int32_t cid)
	{
		chars.remove(cid);
	}

	void MapChars::clear()
	{
		chars.clear();
	}

	void MapChars::send_movement(int32_t cid, const std::vector<Movement>& movements)
	{
		Optional<OtherChar> otherchar = get_char(cid);
		if (otherchar)
		{
			otherchar->send_movement(movements);
		}
	}

	void MapChars::update_look(int32_t cid, const LookEntry& look)
	{
		Optional<OtherChar> otherchar = get_char(cid);
		if (otherchar)
		{
			otherchar->update_look(look);
		}

	}

	Optional<OtherChar> MapChars::get_char(int32_t cid)
	{
		return chars.get(cid)
			.reinterpret<OtherChar>();
	}
}
