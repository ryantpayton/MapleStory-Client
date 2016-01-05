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
#include "Net\PacketHandler.h"
#include "Character\Inventory\Inventory.h"
#include "Character\Skillbook.h"
#include "Character\Questlog.h"
#include "Character\Monsterbook.h"
#include "Character\Telerock.h"

namespace Net
{
	using Character::Inventory;
	using Character::Skillbook;
	using Character::Questlog;
	using Character::Monsterbook;
	using Character::Telerock;

	// Handler for a packet which contains all character information on first login
	// or warps the player to a different map.
	class SetfieldHandler : public PacketHandler
	{
	public:
		void handle(InPacket& recv) const override;
	private:
		void changemap(InPacket& recv, int32_t) const;
		void setfield(InPacket& recv) const;
		void parseinventory(InPacket& recv, Inventory& invent) const;
		void parseskillbook(InPacket& recv, Skillbook& skills) const;
		void parsequestlog(InPacket& recv, Questlog& quests) const;
		void parsering1(InPacket& recv) const;
		void parsering2(InPacket& recv) const;
		void parsering3(InPacket& recv) const;
		void parseminigame(InPacket& recv) const;
		void parsemonsterbook(InPacket& recv, Monsterbook& monsterbook) const;
		void parsetelerock(InPacket& recv, Telerock& trock) const;
		void parsenewyear(InPacket& recv) const;
		void parseareainfo(InPacket& recv) const;
	};

	// Handler for a packet which moves the player to a different channel.
	class ChangeChannelHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handler for a packet which contains a custom, server-sided map.
	class MapPacketHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};
}