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
	class KeymapHandler83 : public PacketHandler
	{
		void handle(ClientInterface& client, InPacket& recv) const override
		{
			recv.readbyte();

			using::IO::Keytype;
			for (char i = 0; i < 90; i++)
			{
				Keytype type = static_cast<Keytype>(recv.readbyte());
				int action = recv.readint();
				client.getui().getkeyboard().addmapping(i, type, action);
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
		void handle(ClientInterface& client, InPacket& recv) const override
		{
			map<string, pair<char, vector<int>>> macros;
			char size = recv.readbyte();
			for (char i = 0; i < size; i++)
			{
				string name = recv.readascii();
				char shout = recv.readbyte();
				vector<int> skills;
				skills.push_back(recv.readint());
				skills.push_back(recv.readint());
				skills.push_back(recv.readint());
				macros.insert(make_pair(name, make_pair(shout, skills)));
			}
			//uinterface.getkeyboard()->addmacros(macros);
		}
	};
}