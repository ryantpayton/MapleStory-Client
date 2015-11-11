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
#include "OutPacket.h"
#include "SendOpcodes83.h"

namespace Net
{
	// Packet which notifies the server that the connection is still alive.
	class PongPacket : public OutPacket
	{
	public:
		PongPacket() : OutPacket(PONG) {}
	};

	using::Util::NxFileManager;
	// Packet which sends the hash values of all game files to the server.
	class NxCheckPacket : public OutPacket
	{
	public:
		NxCheckPacket(const NxFileManager& nxfiles, uint64_t seed) : OutPacket(HASH_CHECK)
		{
			writech(static_cast<uint8_t>(Util::NUM_FILES));
			for (size_t i = 0; i < Util::NUM_FILES; i++)
			{
				writestr(nxfiles.gethash(i, seed));
			}
		}

		NxCheckPacket(const NxFileManager& nxfiles) : OutPacket(HASH_CHECK)
		{
			writech(static_cast<uint8_t>(Util::NUM_FILES));
			for (size_t i = 0; i < Util::NUM_FILES; i++)
			{
				writestr(nxfiles.gethash(i));
			}
		}
	};
}