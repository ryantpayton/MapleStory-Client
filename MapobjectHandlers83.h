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
	// Handles a packet which tells the client to spawn an npc on the current map.
	class SpawnNpcHandler83 : public PacketHandler
	{
		void handle(ClientInterface& client, InPacket& recv) const
		{
			int32_t oid = recv.readint();
			int32_t id = recv.readint();
			int16_t posx = recv.readshort();
			int16_t cy = recv.readshort();
			bool f = recv.readbool();
			uint16_t fh = recv.readshort();
			int16_t rx = recv.readshort();
			int16_t ry = recv.readshort();

			using::Data::Npcdata;
			const Npcdata& ndata = client.getcache().getnpcs().getnpc(id);
			client.getstage().getnpcs().addnpc(ndata, oid, id, f, fh, vector2d<int32_t>(posx, cy));
		}
	};


	// Handles a packet which tells the client to spawn and control an npc on the current map.
	class SpawnNpcControllerHandler83 : public PacketHandler
	{
		void handle(ClientInterface& client, InPacket& recv) const
		{
			int8_t mode = recv.readbyte();
			int32_t oid = recv.readint();
			if (mode == 0)
			{

			}
			else
			{
				int32_t id = recv.readint();
				int16_t posx = recv.readshort();
				int16_t cy = recv.readshort();
				bool f = recv.readbool();
				uint16_t fh = recv.readshort();
				int16_t rx = recv.readshort();
				int16_t ry = recv.readshort();
				bool minimap = recv.readbool();

				using::Data::Npcdata;
				const Npcdata& ndata = client.getcache().getnpcs().getnpc(id);
				client.getstage().getnpcs().addnpc(ndata, oid, id, f, fh, vector2d<int32_t>(posx, cy));
			}
		}
	};

	/*class spawn_mob_h : public vhandler
	{
		void spawn_mob_h::handle(packet recv)
		{
			int oid = recv.readint();
			bool hascontrol = recv.readbyte() == 5;
			int id = recv.readint();
			recv.skip(22);
			vector2d pos = recv.readpoint();
			char stance = recv.readbyte();
			recv.readshort();
			short fh = recv.readshort();

			bool fadein = false;
			char effect = recv.readbyte();
			if (effect > 0)
			{
				recv.readbyte();
				recv.readshort();
				if (effect == 15)
				{
					recv.readbyte();
				}
			}
			else
			{
				fadein = effect == -2;
			}

			char team = recv.readbyte();
			recv.readint();
			Game::getfield()->getmobs()->addmob(oid, id, hascontrol, pos, stance, fh, effect, fadein, team);
		}
	};

	class spawn_mob_c_h : public vhandler
	{
		void spawn_mob_c_h::handle(packet recv)
		{
			char aggro = recv.readbyte();
			int oid = recv.readint();
			if (aggro == 0)
			{
				return;
			}
			recv.readbyte();
			int id = recv.readint();

			recv.skip(22);

			vector2d pos = recv.readpoint();
			char stance = recv.readbyte();
			recv.readshort();
			short fh = recv.readshort();

			bool fadein = false;
			char effect = recv.readbyte();
			if (effect > 0)
			{
				recv.readbyte();
				recv.readshort();
				if (effect == 15)
				{
					recv.readbyte();
				}
			}
			else
			{
				fadein = effect == -2;
			}

			char team = recv.readbyte();
			recv.readint();
			Game::getfield()->getmobs()->addmob(oid, id, true, pos, stance, fh, effect, fadein, team);
		}
	};

	class clock_h : public vhandler
	{
		void clock_h::handle(packet recv)
		{
			char ctype = recv.readbyte();
			if (ctype == 1)
			{
				char hour = recv.readbyte();
				char minute = recv.readbyte();
				char second = recv.readbyte();
			}
			else if (ctype == 2)
			{
				int time = recv.readint();
			}
			//TO DO
		}
	};

	class show_foreign_effect_h : public vhandler
	{
		void show_foreign_effect_h::handle(packet recv)
		{
			int cid = recv.readint();
			char effect = recv.readbyte();
			Game::getfield()->showchareffect(cid, effect);
		}
	};

	class toggle_ui_h : public vhandler
	{
		void toggle_ui_h::handle(packet recv)
		{
			bool enable = recv.readbool();
			//uinterface.setactive(enable);
		}
	};

	class spawn_player_h : public vhandler
	{
		void spawn_player_h::handle(packet recv)
		{
			int chrid = recv.readint();
			byte level = recv.readbyte();
			string name = recv.readascii();
			string guild = recv.readascii();
			short glogobg = recv.readshort();
			char glogobgcolor = recv.readbyte();
			short glogo = recv.readshort();
			char glogocolor = recv.readbyte();

			recv.skip(8);

			bool morphed = recv.readint() == 2;
			int buffmask1 = recv.readint();
			short buffvalue = 0;
			if (buffmask1 != 0)
			{
				buffvalue = morphed ? recv.readshort() : recv.readbyte();
			}
			int buffmask2 = recv.readint();

			recv.skip(43);

			int mount = recv.readint();

			recv.skip(61);

			short job = recv.readshort();
			maplelook mlook = maplelook(&recv);

			int itemcount = recv.readint(); //count of 5110000, idk 
			int itemeffect = recv.readint();
			int chair = recv.readint();
			vector2d pos = recv.readpoint();
			char stance = recv.readbyte();

			recv.skip(3);

			for (char i = 0; i < 3; i++)
			{
				char available = recv.readbyte();
				if (available == 1)
				{
					bool show = true;
					char byte2 = show ? recv.readbyte() : 0;
					int petid = recv.readint();
					string name = recv.readascii();
					int uqid = recv.readint();
					recv.readint();
					vector2d pos = recv.readpoint();
					char stance = recv.readbyte();
					int fh = recv.readint();
				}
				else
				{
					break;
				}
			}

			int mtlevel = recv.readint();
			int mtexp = recv.readint();
			int mttired = recv.readint();

			//shop stuff, TO DO
			recv.readbyte();
			//shop stuff end

			bool chalkboard = recv.readbool();
			string chalktext = chalkboard ? recv.readascii() : "";

			recv.skip(3);
			char team = recv.readbyte();

			Game::getcache()->getequips()->loadcharlook(&mlook);
			Game::getfield()->getchars()->addchar(chrid, mlook, level, job, name, pos);
		}
	};

	class remove_player_h : public vhandler
	{
		void remove_player_h::handle(packet recv)
		{
			int cid = recv.readint();
			Game::getfield()->getchars()->removechar(cid);
		}
	};

	class player_moved_h : public vhandler
	{
		void player_moved_h::handle(packet recv)
		{
			int cid = recv.readint();
			recv.readint();
			objectmovement moves = objectmovement(&recv);
			Game::getfield()->getchars()->movechar(cid, moves.getmoves());
		}
	};

	class spawn_reactor_h : public vhandler
	{
		void spawn_reactor_h::handle(packet recv)
		{
			int oid = recv.readint();
			int id = recv.readint();
			char state = recv.readbyte();
			vector2d pos = recv.readpoint();
			//uinterface.getfield()->getmap()->addreactor(oid, id, state, pos);
		}
	};

	class remove_reactor_h : public vhandler
	{
		void remove_reactor_h::handle(packet recv)
		{
			int oid = recv.readint();
			char state = recv.readbyte();
			vector2d pos = recv.readpoint();
			//uinterface.getfield()->getmap()->addreactor(oid, id, state, pos);
		}
	};

	class show_mob_hp_h : public vhandler
	{
		void show_mob_hp_h::handle(packet recv)
		{
			int oid = recv.readint();
			char hppercent = recv.readbyte();
			Game::getfield()->getmobs()->sendmobhp(oid, Game::getfield()->getplayer()->getstats()->getstat(MS_LEVEL), hppercent);
		}
	};

	class kill_mob_h : public vhandler
	{
		void kill_mob_h::handle(packet recv)
		{
			int oid = recv.readint();
			char animation = recv.readbyte();
			Game::getfield()->getmobs()->killmob(oid, animation);
		}
	};

	class drop_item_from_mapobject_h : public vhandler
	{
		void drop_item_from_mapobject_h::handle(packet recv)
		{
			char mod = recv.readbyte(); //0 - from player, 1 - normal drop, 2 - spawn, 3 - dissapearing
			int oid = recv.readint();
			bool meso = recv.readbool();
			int itemid = recv.readint();
			int owner = recv.readint();
			char pickuptype = recv.readbyte();
			vector2d dropto = recv.readpoint();
			vector2d dropfrom;
			recv.readint();
			if (mod != 2)
			{
				dropfrom = recv.readpoint();
				recv.readshort();
			}
			else
			{
				dropfrom = dropto;
			}

			if (!meso)
			{
				int64_t expire = recv.readlong();
			}
			bool playerdrop = recv.readbool();
			Game::getfield()->getdrops()->adddrop(oid, itemid, meso, owner, dropfrom, dropto, pickuptype, mod);
		}
	};

	class remove_mapitem_h : public vhandler
	{
		void remove_mapitem_h::handle(packet recv)
		{
			char anim = recv.readbyte();
			int oid = recv.readint();
			moveobject* looter = 0;
			if (anim > 1)
			{
				int cid = recv.readint();
				if (recv.length() > 0)
				{
					char pet = recv.readbyte();
				}

				if (cid == Game::getfield()->getplayer()->getoid())
				{
					Game::getcache()->getsounds()->play(MSN_PICKUP);
				}
				looter = Game::getfield()->getchar(cid);
			}
			Game::getfield()->getdrops()->removedrop(oid, anim, looter);
		}
	};

	class mob_moved_h : public vhandler
	{
		void mob_moved_h::handle(packet recv)
		{
			int oid = recv.readint();
			recv.readbyte();
			char useskill = recv.readbyte();
			char skill = recv.readbyte();
			char skill1 = recv.readbyte();
			char skill2 = recv.readbyte();
			char skill3 = recv.readbyte();
			char skill4 = recv.readbyte();
			vector2d startpos = recv.readpoint();
		}
	};

	class move_mob_response_h : public vhandler
	{
		void move_mob_response_h::handle(packet recv)
		{
			int oid = recv.readint();
			short moveid = recv.readshort();
			bool useskills = recv.readbool();
			short curmp = recv.readshort();
			char skill = recv.readbyte();
			char skilllvl = recv.readbyte();
		}
	};*/
}