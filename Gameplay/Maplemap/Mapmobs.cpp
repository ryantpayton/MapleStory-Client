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
#include "MapMobs.h"

namespace Gameplay
{
	MapMobs::MapMobs() {}

	void MapMobs::addmob(int32_t oid, int32_t id, bool control, int8_t stance, 
		uint16_t fhid, int8_t effect, bool fadein, int8_t team, int32_t x, int32_t y) {

		Mob* mob = getmob(oid);
		if (mob)
			mob->makeactive();
		else
			add(new Mob(oid, id, control, stance, fhid, effect, fadein, team, x, y));
	}

	void MapMobs::killmob(int32_t oid, int8_t animation)
	{
		Mob* mob = getmob(oid);
		if (mob)
		{
			mob->kill(animation);
		}
	}

	void MapMobs::sendmobhp(int32_t oid, int8_t percent, uint16_t playerlevel)
	{
		Mob* mob = getmob(oid);
		if (mob)
		{
			mob->sendhp(percent, playerlevel);
		}
	}

	Mob* MapMobs::getmob(int32_t oid)
	{
		MapObject* mmo = get(oid);
		if (mmo)
			return reinterpret_cast<Mob*>(mmo);
		else
			return nullptr;
	}
}
