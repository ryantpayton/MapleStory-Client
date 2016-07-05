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
#pragma once
#include "MapObjects.h"

#include "../Spawn.h"

#include "../../IO/Cursor.h"

#include <queue>

namespace jrc
{
	class MapNpcs
	{
	public:
		// Draw all npcs on a layer.
		void draw(Layer::Id layer, double viewx, double viewy, float alpha) const;
		// Update all npcs.
		void update(const Physics& physics);

		// Add an npc to the spawn queue.
		void spawn(NpcSpawn&& spawn);
		// Remove the npc with the specified oid;
		void remove(int32_t oid);
		// Remove all npcs.
		void clear();

		// Send mouse input to clickable npcs.
		Cursor::State send_cursor(bool pressed, Point<int16_t> position, Point<int16_t> viewpos);

	private:
		MapObjects npcs;

		std::queue<NpcSpawn> spawns;
	};
}

