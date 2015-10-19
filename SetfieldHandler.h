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
#pragma once
#include "AbstractItemHandler.h"

namespace Net
{
	class SetfieldHandler : public AbstractItemHandler
	{
		void handle(const ParentHandler& parent, InPacket& recv)
		{
			int channel = recv.readint();
			char mode1 = recv.readbyte();
			char mode2 = recv.readbyte();
			if (mode1 == 0 && mode2 == 0)
			{
				changemap(parent, recv, channel);
			}
			else
			{
				setfield(parent, recv);
			}
		}

		void changemap(const ParentHandler& parent, InPacket& recv, int channel)
		{
			recv.skip(3);
			int mapid = recv.readint();
			char portalid = recv.readbyte();
			recv.readlong(); //timestamp
			//parent.getplayer()->getstats()->setspawninfo(mapid, portalid);
			if (channel == parent.getlogin().getchannelid())
			{
				//Game::getcache()->getsounds()->play(MSN_PORTAL);
				//field.setfield(mapid);
				//Game::getwindow()->fade(TRS_MAP);
			}
			else
			{
				//change channel
			}
		}

		void setfield(const ParentHandler& parent, InPacket& recv)
		{
			recv.skip(23);
			int cid = recv.readint();
			const CharEntry& plchar = parent.getlogin().getaccount().getcharbyid(cid);
			if (plchar.getcid() > 0)
			{
				Charstats stats = Charstats(StatsEntry(recv));
				CharLook look;
				parent.getcache().getequips().loadlook(look, plchar.getlook());
				parent.getstage().loadplayer(cid, look, stats);

				char buddycap = recv.readbyte();
				if (recv.readbool())
				{
					string lname = recv.readascii();
				}

				Player& player = parent.getstage().getplayer();
				parseinventory(parent, recv, player.getinvent());
				parseskillbook(parent, recv, player.getskills());
				parsequestlog(parent, recv, player.getquests());
				parseminigame(recv);
				parsering1(recv);
				parsering2(recv);
				parsering3(recv);
				parsetelerock(recv, player.gettrock());
				parsemonsterbook(parent, recv, player.getmonsterbook());
				parsenewyear(recv);
				parseareainfo(recv);

				recv.skip(2);
				recv.readlong(); //timestamp

				parent.getui().remove(UI_CHARSELECT);
				parent.getui().remove(UI_SOFTKEYBOARD);
				//parent.getui()add(UI_QUICKSLOTS);
				//parent.getui()add(UI_STATUSBAR);
				//parent.getui()add(UI_CHATBAR);
				//parent.getui()add(UI_INVENTORY);
				//parent.getui()getbase()->setactive(true);

				//Game::getcache()->getsounds()->play(MSN_GAMEIN);
				int mapid = parent.getstage().getplayer().getstats().getmapid();
				parent.getstage().loadmap(mapid);
				parent.getstage().respawn();
				//Game::getwindow()->fade(TRS_MAP);
			}
		}

		void parseinventory(const ParentHandler& parent, InPacket& recv, Inventory& invent)
		{
			invent.setmeso(recv.readint());
			invent.setslots(IVT_EQUIP, static_cast<uint8_t>(recv.readbyte()));
			invent.setslots(IVT_USE, static_cast<uint8_t>(recv.readbyte()));
			invent.setslots(IVT_SETUP, static_cast<uint8_t>(recv.readbyte()));
			invent.setslots(IVT_ETC, static_cast<uint8_t>(recv.readbyte()));
			invent.setslots(IVT_CASH, static_cast<uint8_t>(recv.readbyte()));
			recv.skip(8);

			for (char i = 0; i < 3; i++)
			{
				InventoryType inv = (i == 0) ? IVT_EQUIPPED : IVT_EQUIP;
				short pos = recv.readshort();
				while (pos != 0)
				{
					short slot = (i == 1) ? -pos : pos;
					Item* toadd = parseitem(parent.getcache(), recv, inv);
					invent.add(toadd, inv, slot);
					pos = recv.readshort();
				}
			}
			recv.skip(2);

			InventoryType toparse[4] = { IVT_USE, IVT_SETUP, IVT_ETC, IVT_CASH };
			for (char i = 0; i < 4; i++)
			{
				InventoryType inv = toparse[i];
				char pos = recv.readbyte();
				while (pos != 0)
				{
					Item* toadd = parseitem(parent.getcache(), recv, inv);
					invent.add(toadd, inv, pos);
					pos = recv.readbyte();
				}
			}
		}

		void parseskillbook(const ParentHandler& parent, InPacket& recv, Skillbook& skills)
		{
			short size = recv.readshort();
			for (short i = 0; i < size; i++)
			{
				int skillid = recv.readint();
				int level = recv.readint();
				int64_t expiration = recv.readlong();
				bool fourthjob = ((skillid % 100000) / 10000 == 2);
				int masterlevel;
				if (fourthjob)
				{
					masterlevel = recv.readint();
				}
				else
				{
					masterlevel = -1;
				}
				skills.setskill(skillid, level, masterlevel, expiration);
			}
			size = recv.readshort();
			for (short i = 0; i < size; i++)
			{
				int skillid = recv.readint();
				int cooldown = recv.readshort();
				skills.setcd(skillid, cooldown);
			}
		}

		void parsequestlog(const ParentHandler& parent, InPacket& recv, Questlog& quests)
		{
			short size = recv.readshort();
			for (short i = 0; i < size; i++)
			{
				short qid = recv.readshort();
				string qdata = recv.readascii();
				if (quests.isstarted(qid))
				{
					short qidl = quests.getlaststarted();
					quests.addprogress(qidl, qid, qdata);
					i--;
				}
				else
				{
					quests.addstarted(qid, qdata);
				}
			}
			map<short, int64_t> completed;
			size = recv.readshort();
			for (short i = 0; i < size; i++)
			{
				short qid = recv.readshort();
				int64_t time = recv.readlong();
				quests.addcompleted(qid, time);
			}
		}

		void parsemonsterbook(const ParentHandler& parent, InPacket& recv, Monsterbook& monsterbook)
		{
			int cover = recv.readint();
			monsterbook.setcover(cover);
			recv.skip(1);
			short size = recv.readshort();
			for (short i = 0; i < size; i++)
			{
				short cid = recv.readshort();
				char mblv = recv.readbyte();
				monsterbook.addcard(cid, mblv);
			}
		}

		void parsering1(InPacket& recv)
		{
			short rsize = recv.readshort();
			for (short i = 0; i < rsize; i++)
			{
				recv.readint();
				recv.readpadascii(13);
				recv.readint();
				recv.readint();
				recv.readint();
				recv.readint();
			}
		}

		void parsering2(InPacket& recv)
		{
			short rsize = recv.readshort();
			for (short i = 0; i < rsize; i++)
			{
				recv.readint();
				recv.readpadascii(13);
				recv.readint();
				recv.readint();
				recv.readint();
				recv.readint();
				recv.readint();
			}
		}

		void parsering3(InPacket& recv)
		{
			short rsize = recv.readshort();
			for (short i = 0; i < rsize; i++)
			{
				recv.readint();
				recv.readint();
				recv.readint();
				recv.readshort();
				recv.readint();
				recv.readint();
				recv.readpadascii(13);
				recv.readpadascii(13);
			}
		}

		void parseminigame(InPacket& recv)
		{
			short mgsize = recv.readshort();
			for (short i = 0; i < mgsize; i++)
			{
				//minigameinfo, odin doesn't have it
			}
		}

		void parsetelerock(InPacket& recv, Telerock& trock)
		{
			for (char i = 0; i < 5; i++)
			{
				int mapid = recv.readint();
				trock.addlocation(mapid);
			}
			for (char i = 0; i < 10; i++)
			{
				int mapid = recv.readint();
				trock.addviplocation(mapid);
			}
		}

		void parsenewyear(InPacket& recv)
		{
			short nysize = recv.readshort();
			for (short i = 0; i < nysize; i++)
			{
				//newyearsinfo, odin doesn't have it
			}
		}

		void parseareainfo(InPacket& recv)
		{
			map<short, string> areainfo;
			short arsize = recv.readshort();
			for (short i = 0; i < arsize; i++)
			{
				short area = recv.readshort();
				areainfo[area] = recv.readascii();
			}
		}
	};
}