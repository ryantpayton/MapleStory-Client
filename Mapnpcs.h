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
#include "Mapobjects.h"
#include "Npc.h"

namespace Gameplay
{
	// Collection of npcs on the current map.
	class Mapnpcs : public Mapobjects
	{
	public:
		Mapnpcs() {}
		// Add an npc using the specified values.
		// Parameters: Npcdata&(shared game data among the same npcs), int32_t(id), int32_t(oid)
		// bool(is flipped), uint16_t(starting foothoold), vector2d(position)
		void addnpc(const Npcdata&, int32_t, int32_t, bool, uint16_t, vector2d<int32_t>);
	};
}

