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
#include "MapNpcs.h"

#include "..\..\Net\Packets\NpcInteractionPackets.h"

namespace jrc
{
	void MapNpcs::draw(int8_t layer, double viewx, double viewy, float alpha) const
	{
		npcs.draw(layer, viewx, viewy, alpha);
	}

	void MapNpcs::update(const Physics& physics)
	{
		npcs.update(physics);
	}

	void MapNpcs::send_spawn(const NpcSpawn& spawn, const Physics& physics)
	{
		int32_t oid = spawn.get_oid();
		Optional<Npc> npc = getnpc(oid);
		if (npc)
		{
			npc->makeactive();
		}
		else
		{
			npcs.add(
				spawn.instantiate(physics)
			);
		}
	}

	void MapNpcs::remove_npc(int32_t oid)
	{
		getnpc(oid)
			.if_present(&Npc::deactivate);
	}

	void MapNpcs::clear()
	{
		npcs.clear();
	}

	Cursor::State MapNpcs::send_cursor(bool pressed, Point<int16_t> position, Point<int16_t> viewpos)
	{
		for (auto& mmo : npcs)
		{
			Npc* npc = static_cast<Npc*>(mmo.second.get());
			if (npc && npc->is_active() && npc->inrange(position, viewpos))
			{
				if (pressed)
				{
					// TODO: try finding dialogue first
					TalkToNPCPacket(npc->get_oid())
						.dispatch();
					return Cursor::IDLE;
				}
				else
				{
					return Cursor::CANCLICK;
				}
			}
		}
		return Cursor::IDLE;
	}

	Optional<Npc> MapNpcs::getnpc(int32_t oid)
	{
		return npcs.get(oid)
			.reinterpret<Npc>();
	}
}
