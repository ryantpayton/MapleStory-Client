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

#include "Net\Session.h"
#include "Net\Packets\NpcInteractionPackets.h"

namespace Gameplay
{
	using Net::Session;

	void MapNpcs::sendspawn(const NpcSpawn& spawn, const Physics& physics)
	{
		int32_t oid = spawn.getoid();
		Optional<Npc> npc = getnpc(oid);
		if (npc)
		{
			npc->makeactive();
		}
		else
		{
			Npc* newnpc = spawn.instantiate(physics);
			add(newnpc);
		}
	}

	void MapNpcs::removenpc(int32_t oid)
	{
		getnpc(oid)
			.ifpresent(&Npc::deactivate);
	}

	Cursor::State MapNpcs::sendmouse(bool pressed, Point<int16_t> position, Point<int16_t> viewpos)
	{
		for (auto& mmo : objects)
		{
			Optional<Npc> npc = getnpc(mmo.first);
			if (npc && npc->isactive() && npc->inrange(position, viewpos))
			{
				if (pressed)
				{
					// try finding dialogue first
					using Net::TalkToNPCPacket;
					Session::get().dispatch(TalkToNPCPacket(npc->getoid()));
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
		return get(oid)
			.reinterpret<Npc>();
	}
}
