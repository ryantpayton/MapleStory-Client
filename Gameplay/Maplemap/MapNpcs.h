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
#include "Npc.h"

namespace Gameplay
{
	class MapNpcs : public MapObjects
	{
	public:
		MapNpcs();
		// Spawn an npc on the current map.
		// Parameters: int32_t(id), int32_t(oid), bool(is flipped), 
		// uint16_t(starting foothoold), bool(control), int32_t(x), int32_t(y)
		void addnpc(int32_t, int32_t, bool, uint16_t, bool, int32_t, int32_t);

	private:
		Npc* getnpc(int32_t);
	};
}

