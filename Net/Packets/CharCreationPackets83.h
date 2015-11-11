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
#include "Net\SendOpcodes83.h"

namespace Net
{
	class NameCharPacket83 : public OutPacket
	{
	public:
		NameCharPacket83(string name) : OutPacket(NAME_CHAR)
		{
			writestr(name);
		}
	};

	class CreateCharPacket83 : public OutPacket
	{
	public:
		CreateCharPacket83(string name, short job, int face, int hair, uint8_t hairc, 
			uint8_t skin, int top, int bot, int shoes, int weapon, bool female) : OutPacket(CREATE_CHAR) {

			writestr(name);
			writeint(job);
			writeint(face);
			writeint(hair);
			writeint(hairc);
			writeint(skin);
			writeint(top);
			writeint(bot);
			writeint(shoes);
			writeint(weapon);
			writech(female);
		}
	};
}