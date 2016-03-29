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
#include "Net\Session.h"

namespace Net
{
	// Handles a packet which tells the client to spawn an npc on the current map.
	class SpawnNpcHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handles a packet which tells the client to spawn a controlled npc on the current map.
	class SpawnNpcControllerHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handles a packet which tells the client to spawn a mob.
	class SpawnMobHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handles a packet which tells the client to spawn and control a mob.
	class SpawnMobControllerHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	//  Handles a packet which updates mob movement with the client.
	class MobMovedHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handles a packet which updates a mob's hp with the client.
	class ShowMobHpHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handles a packet which removes a mob from the current map.
	class KillMobHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class SpawnCharHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class RemoveCharHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class CharMovedHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class UpdateCharLookHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class SpawnPetHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class DropItemHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class RemoveDropHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};
}