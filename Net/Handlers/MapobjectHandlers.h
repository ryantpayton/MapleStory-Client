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
#include "Net\Session.h"
#include "AbstractMovementHandler83.h"
#include "Gameplay\Stage.h"

namespace Net
{
	// Handles a packet which tells the client to spawn an npc on the current map.
	class SpawnNpcHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			int32_t oid = recv.readint();
			int32_t id = recv.readint();
			vector2d<int16_t> position = recv.readpoint();
			bool flip = recv.readbool();
			uint16_t fh = recv.readshort();

			Gameplay::Stage::getnpcs().addnpc(id, oid, flip, fh, true, position);
		}
	};

	// Handles a packet which tells the client to spawn a mob.
	class SpawnMobHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			int32_t oid = recv.readint();
			int32_t id = recv.readint();
			vector2d<int16_t> position = recv.readpoint();
			int8_t stance = recv.readbyte();
			uint16_t fh = recv.readshort();
			bool fadein = recv.readbool();
			int8_t team = recv.readbyte();

			Gameplay::Stage::getmobs().addmob(oid, id, true, stance, fh, fadein, team, position);
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

			vector<MovementFragment> movements;
			uint8_t nummoves = recv.readbyte();
			for (uint8_t i = 0; i < nummoves; i++)
			{
				movements.push_back(parsemovement(recv));
			}

			Gameplay::Stage::getchars().movechar(cid, movements);
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

	class DropItemHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			int8_t mode = recv.readbyte();
			int32_t oid = recv.readint();
			bool meso = recv.readbool();
			int32_t itemid = recv.readint();
			int32_t owner = recv.readint();
			int8_t pickuptype = recv.readbyte();
			vector2d<int16_t> dropto = recv.readpoint();
			vector2d<int16_t> dropfrom;
			if (mode != 2)
				dropfrom = recv.readpoint();
			else
				dropfrom = dropto;
			bool playerdrop = recv.readbool();

			Gameplay::Stage::getdrops().adddrop(oid, itemid, meso, owner, dropfrom, dropto, pickuptype, mode, playerdrop);
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