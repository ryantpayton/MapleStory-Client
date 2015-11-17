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
#include "AbstractMovementHandler83.h"
#include "Gameplay\Stage.h"

namespace Net
{
	// Handles a packet which tells the client to spawn an npc on the current map.
	class SpawnNpcHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			int32_t oid = recv.readint();
			int32_t id = recv.readint();
			int16_t posx = recv.readshort();
			int16_t posy = recv.readshort();
			bool f = recv.readbool();
			uint16_t fh = recv.readshort();
			int16_t rx = recv.readshort();
			int16_t ry = recv.readshort();

			Gameplay::Stage::getnpcs().addnpc(id, oid, f, fh, false, posx, posy);
		}
	};


	// Handles a packet which tells the client to spawn and control an npc on the current map.
	class SpawnNpcControllerHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
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
				int16_t posy = recv.readshort();
				bool f = recv.readbool();
				uint16_t fh = recv.readshort();
				int16_t rx = recv.readshort();
				int16_t ry = recv.readshort();
				bool minimap = recv.readbool();

				Gameplay::Stage::getnpcs().addnpc(id, oid, f, fh, true, posx, posy);
			}
		}
	};

	// Handles a packet which tells the client to spawn a mob.
	class SpawnMobHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			int32_t oid = recv.readint();
			bool hascontrol = recv.readbyte() == 5;
			int32_t id = recv.readint();
			recv.skip(22);
			int16_t posx = recv.readshort();
			int16_t posy = recv.readshort();
			int8_t stance = recv.readbyte();
			recv.readshort();
			uint16_t fh = recv.readshort();
			bool fadein = false;
			int8_t effect = recv.readbyte();
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
			int8_t team = recv.readbyte();
			recv.skip(4);

			Gameplay::Stage::getmobs().addmob(oid, id, hascontrol, stance, fh, effect, fadein, team, posx, posy);
		}
	};

	// Handles a packet which tells the client to spawn a mob and control it.
	class SpawnMobControllerHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			int8_t aggro = recv.readbyte();
			int32_t oid = recv.readint();
			if (aggro == 0)
			{
				return;
			}
			recv.readbyte();
			int32_t id = recv.readint();
			recv.skip(22);
			int16_t posx = recv.readshort();
			int16_t posy = recv.readshort();
			int8_t stance = recv.readbyte();
			recv.readshort();
			uint16_t fh = recv.readshort();
			bool fadein = false;
			int8_t effect = recv.readbyte();
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
			int8_t team = recv.readbyte();
			recv.skip(4);

			Gameplay::Stage::getmobs().addmob(oid, id, true, stance, fh, effect, fadein, team, posx, posy);
		}
	};

	class ShowMobHpHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			int32_t oid = recv.readint();
			int8_t hppercent = recv.readbyte();

			uint16_t playerlevel = Gameplay::Stage::getplayer().getstats().getstat(Character::MS_LEVEL);
			Gameplay::Stage::getmobs().sendmobhp(oid, hppercent, playerlevel);
		}
	};

	class KillMobHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			int32_t oid = recv.readint();
			int8_t animation = recv.readbyte();

			Gameplay::Stage::getmobs().killmob(oid, animation);
		}
	};
	/*
	

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
	};*/

	class SpawnCharHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			int32_t cid = recv.readint();
			uint8_t level = recv.readbyte();
			string name = recv.readascii();

			string guild = recv.readascii();
			int16_t glogobg = recv.readshort();
			int8_t glogobgcolor = recv.readbyte();
			int16_t glogo = recv.readshort();
			int8_t glogocolor = recv.readbyte();

			recv.skip(8);

			bool morphed = recv.readint() == 2;
			int32_t buffmask1 = recv.readint();
			int16_t buffvalue = 0;
			if (buffmask1 != 0)
			{
				buffvalue = morphed ? recv.readshort() : recv.readbyte();
			}
			int32_t buffmask2 = recv.readint();

			recv.skip(43);

			int32_t mount = recv.readint();

			recv.skip(61);

			int16_t job = recv.readshort();
			LookEntry look = LookEntry(recv);

			int32_t itemcount = recv.readint(); //count of 5110000, idk 
			int32_t itemeffect = recv.readint();
			int32_t chair = recv.readint();
			int32_t px = recv.readshort();
			int32_t py = recv.readshort();
			int8_t stance = recv.readbyte();

			recv.skip(3);

			for (size_t i = 0; i < 3; i++)
			{
				int8_t available = recv.readbyte();
				if (available == 1)
				{
					bool show = true;
					int8_t byte2 = show ? recv.readbyte() : 0;
					int32_t petid = recv.readint();
					string name = recv.readascii();
					int32_t uqid = recv.readint();
					recv.readint();
					vector2d<int16_t> pos = recv.readpoint();
					int8_t stance = recv.readbyte();
					int32_t fh = recv.readint();
				}
				else
				{
					break;
				}
			}

			int32_t mtlevel = recv.readint();
			int32_t mtexp = recv.readint();
			int32_t mttired = recv.readint();

			//shop stuff, TO DO
			recv.readbyte();
			//shop stuff end

			bool chalkboard = recv.readbool();
			string chalktext = chalkboard ? recv.readascii() : "";

			recv.skip(3);
			int8_t team = recv.readbyte();

			Gameplay::Stage::getchars().addchar(cid, look, level, job, name, stance, vector2d<int32_t>(px, py));
		}
	};

	class RemoveCharHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			int32_t cid = recv.readint();
			Gameplay::Stage::getchars().removechar(cid);
		}
	};

	class MoveCharHandler83 : public AbstractMovementHandler83
	{
		void handle(InPacket& recv) const override
		{
			int32_t cid = recv.readint();
			recv.skip(4);

			MovementInfo movements;
			parsemovement(recv, movements);

			Gameplay::Stage::getchars().movechar(cid, movements);
		}
	};

	/*class spawn_reactor_h : public vhandler
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
	};*/
}