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
#include "Graphics\Text.h"

namespace Net
{
	using Graphics::Text;

	// Handler for a packet which contains status messages.
	class ShowStatusInfoHandler : public PacketHandler
	{
	public:
		void handle(InPacket& recv) const override;
	private:
		void showstatus(Text::Color color, string message) const;
	};

	// Handler for a packet which contains chat messages.
	class ChatReceivedHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};
}