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
#include "IO\UI.h"

namespace Net
{
	class KeymapHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			recv.readbyte();

			for (uint8_t i = 0; i < 90; i++)
			{
				uint8_t type = recv.readbyte();
				int32_t action = recv.readint();
				IO::UI::addkeymapping(i, type, action);
			}

			/*quickslot* qslot = uinterface.getelement<quickslot>(UI_QUICKSLOTS);
			if (qslot)
			{
			qslot->loadicons();
			}*/
		}
	};

	class SkillmacrosHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			map<string, pair<int8_t, vector<int32_t>>> macros;
			uint8_t size = recv.readbyte();
			for (uint8_t i = 0; i < size; i++)
			{
				string name = recv.readascii();
				int8_t shout = recv.readbyte();
				vector<int32_t> skills;
				skills.push_back(recv.readint());
				skills.push_back(recv.readint());
				skills.push_back(recv.readint());
				macros.insert(make_pair(name, make_pair(shout, skills)));
			}
			//uinterface.getkeyboard()->addmacros(macros);
		}
	};
}