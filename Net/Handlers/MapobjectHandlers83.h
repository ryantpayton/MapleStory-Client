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
			recv.readshort(); // 'rx'
			recv.readshort(); // 'ry'

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
				recv.readshort(); // 'rx'
				recv.readshort(); // 'ry'
				recv.readbool(); // 'minimap'

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
			int32_t id = recv.readint();
			int16_t posx = recv.readshort();
			int16_t posy = recv.readshort();
			int8_t stance = recv.readbyte();
			uint16_t fh = recv.readshort();
			bool fadein = recv.readbool();
			int8_t team = recv.readbyte();

			Gameplay::Stage::getmobs().addmob(oid, id, stance, fh, fadein, team, posx, posy);
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

			recv.readascii(); // guildname
			recv.readshort(); // guildlogobg
			recv.readbyte(); // guildlogobgcolor
			recv.readshort(); // guildlogo
			recv.readbyte(); // guildlogocolor

			recv.skip(8);

			bool morphed = recv.readint() == 2;
			int32_t buffmask1 = recv.readint();
			int16_t buffvalue = 0;
			if (buffmask1 != 0)
			{
				buffvalue = morphed ? recv.readshort() : recv.readbyte();
			}
			recv.readint(); // buffmask 2

			recv.skip(43);

			recv.readint(); // 'mount'

			recv.skip(61);

			int16_t job = recv.readshort();
			LookEntry look = Session::getlogin().parselook(recv);

			recv.readint(); //count of 5110000 
			recv.readint(); // 'itemeffect'
			recv.readint(); // 'chair'

			vector2d<int16_t> position = recv.readpoint();
			int8_t stance = recv.readbyte();

			recv.skip(3);

			for (size_t i = 0; i < 3; i++)
			{
				int8_t available = recv.readbyte();
				if (available == 1)
				{
					recv.readbyte(); // 'byte2'
					recv.readint(); // petid
					recv.readascii(); // name
					recv.readint(); // unique id
					recv.readint();
					recv.readpoint(); // pos
					recv.readbyte(); // stance
					recv.readint(); // fhid
				}
				else
				{
					break;
				}
			}

			recv.readint(); // mountlevel
			recv.readint(); // mountexp
			recv.readint(); // mounttiredness

			//shop stuff, TO DO
			recv.readbyte();
			//shop stuff end

			bool chalkboard = recv.readbool();
			string chalktext = chalkboard ? recv.readascii() : "";

			recv.skip(3);
			recv.readbyte(); // team

			Gameplay::Stage::getchars().addchar(cid, look, level, job, name, stance, position);
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
			MovementFragment movement = parsemovement(recv);

			Gameplay::Stage::getchars().movechar(cid, movement);
		}
	};

	class SpawnPetHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			using Character::Char;
			Char* character = Gameplay::Stage::getcharacter(recv.readint());

			if (!character)
				return;

			uint8_t petindex = recv.readbyte();
			int8_t mode = recv.readbyte();

			if (mode == 1)
			{
				recv.skip(1);
				int32_t itemid = recv.readint();
				string name = recv.readascii();
				int32_t uniqueid = recv.readint();
				recv.skip(4);
				vector2d<int16_t> pos = recv.readpoint();
				uint8_t stance = recv.readbyte();
				int32_t fhid = recv.readint();

				character->addpet(petindex, itemid, name, uniqueid, pos, stance, fhid);
			}
			else if (mode == 0)
			{
				bool hunger = recv.readbool();

				character->removepet(petindex, hunger);
			}
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
	};*/

	
	class DropItemHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			int8_t mode = recv.readbyte(); //0 - from player, 1 - normal drop, 2 - spawn, 3 - dissapearing
			int32_t oid = recv.readint();
			bool meso = recv.readbool();
			int32_t itemid = recv.readint();
			int32_t owner = recv.readint();
			int8_t pickuptype = recv.readbyte();
			vector2d<int16_t> dropto = recv.readpoint();
			vector2d<int16_t> dropfrom;
			recv.readint();
			if (mode != 2)
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
				//int64_t expire = recv.readlong();
			}
			recv.readbool(); // playerdrop
			Gameplay::Stage::getdrops().adddrop(oid, itemid, meso, owner, dropfrom, dropto, pickuptype, mode);
		}
	};

	class RemoveDropHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			int8_t anim = recv.readbyte();
			int32_t oid = recv.readint();
			const Gameplay::PhysicsObject* looter = nullptr;
			if (anim > 1)
			{
				int32_t cid = recv.readint();
				if (recv.length() > 0)
				{
					//int8_t pet = recv.readbyte();
				}
				else
				{
					Character::Char* charlooter = Gameplay::Stage::getcharacter(cid);
					if (charlooter)
						looter = &charlooter->getphobj();
				}
				/*if (cid == Game::getfield()->getplayer()->getoid())
				{
					Game::getcache()->getsounds()->play(MSN_PICKUP);
				}
				looter = Game::getfield()->getchar(cid);*/
			}
			Gameplay::Stage::getdrops().removedrop(oid, anim, looter);
		}
	};
}