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
#pragma once
#include "MapObjects.h"
#include "Mob.h"

#include "Gameplay\Attack.h"

#include <map>

namespace Gameplay
{
	using std::map;

	class MapMobs : public MapObjects
	{
	public:
		MapMobs();

		void update(const Physics& physics) override;

		void addmob(int32_t oid, int32_t mobid, bool control, int8_t stance, 
			uint16_t fhid, bool fadein, int8_t team, vector2d<int16_t> position);
		void killmob(int32_t oid, int8_t effect);
		void sendmobhp(int32_t oid, int8_t percent, uint16_t playerlevel);
		void sendattack(Attack attack);

	private:
		void applyattack(const Attack& attack);
		Mob* getmob(int32_t oid);

		map<uint8_t, Attack> attackschedule;
		uint8_t raid;
	};
}

