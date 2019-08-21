//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../PacketHandler.h"

#include "../Character/Player.h"

namespace ms
{
	// Handler for a packet which contains all character information on first login
	// or warps the player to a different map.
	class SetfieldHandler : public PacketHandler
	{
	public:
		void handle(InPacket& recv) const override;

	private:
		void transition(int32_t mapid, uint8_t portalid) const;
		void change_map(InPacket& recv, int32_t map_id) const;
		void set_field(InPacket& recv) const;

		void parse_inventory(InPacket& recv, Inventory& inventory) const;
		void parse_skillbook(InPacket& recv, Skillbook& skills) const;
		void parse_cooldowns(InPacket& recv, Player& player) const;
		void parse_questlog(InPacket& recv, Questlog& quests) const;
		void parse_ring1(InPacket& recv) const;
		void parse_ring2(InPacket& recv) const;
		void parse_ring3(InPacket& recv) const;
		void parse_minigame(InPacket& recv) const;
		void parse_monsterbook(InPacket& recv, Monsterbook& monsterbook) const;
		void parse_telerock(InPacket& recv, Telerock& telerock) const;
		void parse_nyinfo(InPacket& recv) const;
		void parse_areainfo(InPacket& recv) const;
	};
}