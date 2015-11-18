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
#include <memory>

namespace Net
{
	using std::unique_ptr;

	// Maximum number of handler classes needed for now.
	const uint16_t NUM_HANDLERS = 500;

	// Class which contains the array of handler classes to use. Also responsible for dealing with errors.
	class PacketHandler83
	{
	public:
		// Register all handlers.
		PacketHandler83();
		// Empty destructor.
		~PacketHandler83();

		// Handle a packet.
		void handle(InPacket&) const;

	private:
		unique_ptr<PacketHandler> handlers[NUM_HANDLERS];
	};
}

