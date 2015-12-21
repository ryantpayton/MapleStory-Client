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
#include "Net\OutPacket.h"
#include "Net\SendOpcodes.h"

namespace Net
{
	// Packet which requests reserving a name for the character to be created.
	class NameCharPacket : public OutPacket
	{
	public:
		NameCharPacket(string name) : OutPacket(NAME_CHAR)
		{
			writestr(name);
		}
	};

	// Packets which requests creation of a character with the specified stats.
	class CreateCharPacket : public OutPacket
	{
	public:
		CreateCharPacket(string name, uint16_t job, int32_t face, int32_t hair, 
			uint8_t hairc, uint8_t skin, int32_t top, int32_t bot, int32_t shoes, 
			int32_t weapon, bool female) : OutPacket(CREATE_CHAR) {

			writestr(name);
			writeint(job);
			writeint(face);
			writeint(hair);
			writeint(hairc);
			writech(skin);
			writeint(top);
			writeint(bot);
			writeint(shoes);
			writeint(weapon);
			writech(female);
		}
	};
}