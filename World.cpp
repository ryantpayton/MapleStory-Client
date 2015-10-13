/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "World.h"

namespace Net
{
	World::World(uint8_t id, InPacket* recv)
	{
		wid = id;
		name = recv->readascii();
		flag = recv->readbyte();
		message = recv->readascii();
		recv->readbyte();
		recv->readbyte();
		recv->readbyte();
		recv->readbyte();
		recv->readbyte();
		channelcount = recv->readbyte();
		for (uint8_t i = 0; i < channelcount; i++)
		{
			recv->readascii();
			recv->readint();
			chloads.push_back(recv->readbyte());
			recv->readshort();
		}
	}

	uint8_t World::getid()
	{
		return wid;
	}

	uint8_t World::getchcount()
	{
		return channelcount;
	}

	vector<char>* World::getchloads()
	{
		return &chloads;
	}
}
