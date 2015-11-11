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

namespace Gameplay
{
	class MapMobs : public MapObjects
	{
	public:
		MapMobs();
		void addmob(int32_t, int32_t, bool, int8_t, uint16_t, int8_t, bool, int8_t, int32_t, int32_t);
		void killmob(int32_t, int8_t);
		void sendmobhp(int32_t, int8_t, uint16_t);

	private:
		Mob* getmob(int32_t);
	};
}

