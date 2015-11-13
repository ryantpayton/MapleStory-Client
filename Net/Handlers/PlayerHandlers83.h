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
	// Handles a packet which notifies the client of changes in character stats.
	class StatschangedHandler83 : public PacketHandler
	{
		void handle(ClientInterface& client, InPacket& recv) const override
		{
			bool itemreaction = recv.readbool();
			int32_t updatemask = recv.readint();
			if (updatemask > 0)
			{
				for (size_t i = 0; i < 20; i++)
				{
					using::Character::Player;
					Player& player = client.getstage().getplayer();
					Maplestat stat = Character::statvalues[i];

					if (updatemask & stat)
					{
						switch (stat)
						{
						case Character::MS_SKIN:
							player.getlook().setbody(static_cast<int8_t>(recv.readshort()));
							break;
						case Character::MS_FACE:
							player.getlook().setface(recv.readint());
							break;
						case Character::MS_HAIR:
							player.getlook().sethair(recv.readint());
							break;
						case Character::MS_LEVEL:
							player.getstats().setstat(stat, static_cast<uint8_t>(recv.readbyte()));
							//parent.getstage().showchareffect(0);
							break;
						case Character::MS_EXP:
							player.getstats().setexp(recv.readint());
							break;
						case Character::MS_MESO:
							player.getinvent().setmeso(recv.readint());
							break;
						case Character::MS_AP:
							player.getstats().setstat(stat, recv.readshort());
							/*if (uinterface.getelement(UI_STATSINFO))
							{
							uinterface.getelement(UI_STATSINFO)->sendbool(player->getstats()->getstat(MS_AP) > 0);
							}*/
							break;
						default:
							player.getstats().setstat(stat, recv.readshort());
							player.recalcstats(false);
							break;
						}
					}
				}
			}
			else
			{
				client.getui().enable();
			}
		}
	};

	// Handles a packet which forces a stats recalculation.
	class StatresetHandler83 : public PacketHandler
	{
		void handle(ClientInterface& client, InPacket& recv) const override
		{
			client.getstage().getplayer().recalcstats(false);
		}
	};

	// Handles a packet which notifies the client of changes in a character's skills.
	class UpdateskillsHandler83 : public PacketHandler
	{
		void handle(ClientInterface& client, InPacket& recv) const override
		{
			recv.skip(3);

			int32_t skillid = recv.readint();
			int32_t level = recv.readint();
			int32_t masterlevel = recv.readint();
			int64_t expire = recv.readlong();

			client.getstage().getplayer().getskills().setskill(skillid, level, masterlevel, expire);
		}
	};
}