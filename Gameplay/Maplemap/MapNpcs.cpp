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
#include "MapNpcs.h"

namespace Gameplay
{
	MapNpcs::MapNpcs() {}

	void MapNpcs::addnpc(int32_t id, int32_t oid, bool flip, 
		uint16_t fhid, bool control, int16_t x, int16_t y) {

		Npc* npc = getnpc(oid);
		if (!npc)
		{
			add(new Npc(id, oid, flip, fhid, control, x, y));
		}
	}

	Npc* MapNpcs::getnpc(int32_t oid)
	{
		MapObject* mmo = get(oid);
		if (mmo)
			return reinterpret_cast<Npc*>(mmo);
		else
			return nullptr;
	}
}
