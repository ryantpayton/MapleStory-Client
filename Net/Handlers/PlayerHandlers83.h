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
#include "AbstractItemHandler83.h"

namespace Net
{
	// Handles a packet which notifies the client of changes in character stats.
	class StatschangedHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handles a packet which forces a stats recalculation.
	class StatresetHandler83 : public PacketHandler
	{
		void handle(InPacket&) const override;
	};

	// Handles a packet which notifies the client of changes in a character's skills.
	class UpdateskillsHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handler for a packet which modifies the player's inventory.
	class ModifyInventoryHandler : public AbstractItemHandler83
	{
		void handle(InPacket& recv) const override;
	};
}