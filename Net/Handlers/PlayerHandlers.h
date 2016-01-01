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
#include "Character\Buffstat.h"

namespace Net
{
	// Handles a packet which notifies the client of changes in character stats.
	class ChangeStatsHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handles a packet which forces a stats recalculation.
	class RecalculateStatsHandler : public PacketHandler
	{
		void handle(InPacket&) const override;
	};

	// Handles a packet which notifies the client of changes in a character's skills.
	class UpdateskillsHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	using Character::Buffstat;

	// Handles a packet which notifies the client that a buff was applied to the player.
	class ApplyBuffHandler : public PacketHandler
	{
	public:
		void handle(InPacket& recv) const override;
	private:
		void handlebuff(InPacket& recv, Buffstat buff) const;
	};
}