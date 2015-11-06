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
#include "PacketHandler.h"

namespace Net
{
	// Maximum number of handler classes needed for version 83.
	const uint16_t NUM_HANDLERS = 355;
	// Class which contains the array of handler classes to use. Also responsible for dealing with errors.
	class PacketHandler83
	{
	public:
		// Initialise array and register all handlers.
		PacketHandler83();
		// Destroy all handlers.
		~PacketHandler83();
		// Handle a packet and return if a critical error occured, meaning a bad packet sent by the server.
		// Parameters: ClientInterface&(interface for handlers to change game state), InPacket(packet to handle)
		bool handle(ClientInterface&, InPacket&) const;
	private:
		PacketHandler* handlers[NUM_HANDLERS];
	};
}

