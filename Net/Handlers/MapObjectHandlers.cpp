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
#include "MapObjectHandlers.h"

#include "..\..\Audio\Audio.h"
#include "..\..\Gameplay\Stage.h"
#include "..\..\Gameplay\Spawn.h"

namespace jrc
{
	void SpawnCharHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.read_int();
		uint8_t level = recv.read_byte();
		std::string name = recv.read_string();

		recv.read_string(); // guildname
		recv.read_short(); // guildlogobg
		recv.read_byte(); // guildlogobgcolor
		recv.read_short(); // guildlogo
		recv.read_byte(); // guildlogocolor

		recv.skip(8);

		bool morphed = recv.read_int() == 2;
		int32_t buffmask1 = recv.read_int();
		int16_t buffvalue = 0;
		if (buffmask1 != 0)
		{
			buffvalue = morphed ? recv.read_short() : recv.read_byte();
		}
		recv.read_int(); // buffmask 2

		recv.skip(43);

		recv.read_int(); // 'mount'

		recv.skip(61);

		int16_t job = recv.read_short();
		LookEntry look = Session::get().getlogin().parselook(recv);

		recv.read_int(); //count of 5110000 
		recv.read_int(); // 'itemeffect'
		recv.read_int(); // 'chair'

		Point<int16_t> position = recv.readpoint();
		int8_t stance = recv.read_byte();

		recv.skip(3);

		for (size_t i = 0; i < 3; i++)
		{
			int8_t available = recv.read_byte();
			if (available == 1)
			{
				recv.read_byte(); // 'byte2'
				recv.read_int(); // petid
				recv.read_string(); // name
				recv.read_int(); // unique id
				recv.read_int();
				recv.readpoint(); // pos
				recv.read_byte(); // stance
				recv.read_int(); // fhid
			}
			else
			{
				break;
			}
		}

		recv.read_int(); // mountlevel
		recv.read_int(); // mountexp
		recv.read_int(); // mounttiredness

		//shop stuff, TO DO
		recv.read_byte();
		//shop stuff end

		bool chalkboard = recv.read_bool();
		std::string chalktext = chalkboard ? recv.read_string() : "";

		recv.skip(3);
		recv.read_byte(); // team

		Stage::get().queuespawn(new CharSpawn(cid, look, level, job, name, stance, position));
	}


	void RemoveCharHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.read_int();

		Stage::get().getchars().removechar(cid);
	}


	void SpawnPetHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.read_int();
		Optional<Char> character = Stage::get()
			.getcharacter(cid);
		if (!character)
			return;

		uint8_t petindex = recv.read_byte();
		int8_t mode = recv.read_byte();

		if (mode == 1)
		{
			recv.skip(1);

			int32_t itemid = recv.read_int();
			std::string name = recv.read_string();
			int32_t uniqueid = recv.read_int();

			recv.skip(4);

			Point<int16_t> pos = recv.readpoint();
			uint8_t stance = recv.read_byte();
			int32_t fhid = recv.read_int();

			character->addpet(petindex, itemid, name, uniqueid, pos, stance, fhid);
		}
		else if (mode == 0)
		{
			bool hunger = recv.read_bool();

			character->removepet(petindex, hunger);
		}
	}


	void CharMovedHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.read_int();
		recv.skip(4);
		std::queue<Movement> movements = recv.readqueue<uint8_t, Movement>();

		Stage::get().getchars().movechar(cid, movements);
	}


	void UpdateCharLookHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.read_int();
		recv.read_byte();
		LookEntry look = Session::get().getlogin().parselook(recv);

		Stage::get().getchars().updatelook(cid, look);
	}


	void ShowForeignEffectHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.read_int();
		int8_t effect = recv.read_byte();
		if (effect == 10) // recovery
		{
			recv.read_byte(); // 'amount'
		}
		else if (effect == 13) // card effect
		{
			Stage::get().getcharacter(cid)
				.ifpresent(&Char::showeffectbyid, Char::MONSTER_CARD);
		}
		else if (recv.hasmore()) // skill
		{
			int32_t skillid = recv.read_int();
			recv.read_byte(); // 'direction'
			// 9 more bytes after this

			Stage::get().showbuff(cid, skillid, effect);
		}
		else
		{
			// todo
		}
	}


	void SpawnMobHandler::handle(InPacket& recv) const
	{
		int32_t oid = recv.read_int();
		recv.read_byte(); // 5 if controller == null
		int32_t id = recv.read_int();

		recv.skip(22);

		Point<int16_t> position = recv.readpoint();
		int8_t stance = recv.read_byte();

		recv.skip(2);

		uint16_t fh = recv.read_short();
		int8_t effect = recv.read_byte();

		if (effect > 0)
		{
			recv.read_byte();
			recv.read_short();
			if (effect == 15)
			{
				recv.read_byte();
			}
		}

		int8_t team = recv.read_byte();

		recv.skip(4);

		Stage::get().queuespawn(new MobSpawn(oid, id, 0, stance, fh, effect == -2, team, position));
	}


	void KillMobHandler::handle(InPacket& recv) const
	{
		int32_t oid = recv.read_int();
		int8_t animation = recv.read_byte();

		Stage::get().getmobs().killmob(oid, animation);
	}


	void SpawnMobControllerHandler::handle(InPacket& recv) const
	{
		int8_t mode = recv.read_byte();
		int32_t oid = recv.read_int();
		if (mode == 0)
		{
			Stage::get().getmobs().setcontrol(oid, false);
		}
		else
		{
			if (recv.hasmore())
			{
				recv.skip(1);

				int32_t id = recv.read_int();

				recv.skip(22);

				Point<int16_t> position = recv.readpoint();
				int8_t stance = recv.read_byte();

				recv.skip(2);

				uint16_t fh = recv.read_short();
				int8_t effect = recv.read_byte();

				if (effect > 0)
				{
					recv.read_byte();
					recv.read_short();
					if (effect == 15)
					{
						recv.read_byte();
					}
				}

				int8_t team = recv.read_byte();

				recv.skip(4);

				Stage::get().queuespawn(new MobSpawn(oid, id, mode, stance, fh, effect == -2, team, position));
			}
			else
			{
				// remove monster invisibility, not used in moopledev or solaxia
			}
		}
	}


	void MobMovedHandler::handle(InPacket& recv) const
	{
		int32_t oid = recv.read_int();

		recv.read_byte();
		recv.read_byte(); // useskill
		recv.read_byte(); // skill
		recv.read_byte(); // skill 1
		recv.read_byte(); // skill 2
		recv.read_byte(); // skill 3
		recv.read_byte(); // skill 4

		Point<int16_t> position = recv.readpoint();
		std::queue<Movement> movements = recv.readqueue<uint8_t, Movement>();

		Stage::get().getmobs().movemob(oid, position, movements.front());
	}


	void ShowMobHpHandler::handle(InPacket& recv) const
	{
		int32_t oid = recv.read_int();
		int8_t hppercent = recv.read_byte();
		uint16_t playerlevel = Stage::get().getplayer().getstats().getstat(Maplestat::LEVEL);

		Stage::get().getmobs().sendmobhp(oid, hppercent, playerlevel);
	}


	void SpawnNpcHandler::handle(InPacket& recv) const
	{
		int32_t oid = recv.read_int();
		int32_t id = recv.read_int();
		Point<int16_t> position = recv.readpoint();
		bool flip = recv.read_bool();
		uint16_t fh = recv.read_short();

		recv.read_short(); // 'rx'
		recv.read_short(); // 'ry'

		Stage::get().queuespawn(new NpcSpawn(oid, id, position, flip, fh));
	}


	void SpawnNpcControllerHandler::handle(InPacket& recv) const
	{
		int8_t mode = recv.read_byte();
		int32_t oid = recv.read_int();

		if (mode == 0)
		{
			Stage::get().getnpcs().removenpc(oid);
		}
		else
		{
			int32_t id = recv.read_int();
			Point<int16_t> position = recv.readpoint();
			bool flip = recv.read_bool();
			uint16_t fh = recv.read_short();

			recv.read_short(); // 'rx'
			recv.read_short(); // 'ry'
			recv.read_bool(); // 'minimap'

			Stage::get().queuespawn(new NpcSpawn(oid, id, position, flip, fh));
		}
	}


	void DropLootHandler::handle(InPacket& recv) const
	{
		int8_t mode = recv.read_byte();
		int32_t oid = recv.read_int();
		bool meso = recv.read_bool();
		int32_t itemid = recv.read_int();
		int32_t owner = recv.read_int();
		int8_t pickuptype = recv.read_byte();
		Point<int16_t> dropto = recv.readpoint();

		recv.skip(4);

		Point<int16_t> dropfrom;
		if (mode != 2)
		{
			dropfrom = recv.readpoint();

			recv.skip(2);
		}
		else
		{
			dropfrom = dropto;
		}

		if (!meso)
		{
			recv.skip(8);
		}
		bool playerdrop = !recv.read_bool();

		Stage::get().queuespawn(new DropSpawn(oid, itemid, meso, owner, dropfrom, dropto, pickuptype, mode, playerdrop));
	}


	void RemoveLootHandler::handle(InPacket& recv) const
	{
		int8_t mode = recv.read_byte();
		int32_t oid = recv.read_int();

		Optional<PhysicsObject> looter;
		if (mode > 1)
		{
			int32_t cid = recv.read_int();
			if (recv.length() > 0)
			{
				recv.read_byte(); // pet
			}
			else
			{
				looter = Stage::get().getcharacter(cid)
					.transform(&Char::getphobj);
			}

			Sound(Sound::PICKUP).play();
		}
		Stage::get().getdrops().removedrop(oid, mode, looter.get());
	}
}