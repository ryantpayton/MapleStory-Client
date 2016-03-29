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

#include "Audio\Audio.h"

#include "Gameplay\Stage.h"
#include "Gameplay\Spawn.h"

namespace Net
{
	using Audio::Sound;
	using Character::Char;
	using Gameplay::Stage;
	using Gameplay::NpcSpawn;
	using Gameplay::MobSpawn;
	using Gameplay::DropSpawn;
	using Gameplay::CharSpawn;
	using Gameplay::MapMobs;
	using Gameplay::MapDrops;
	using Gameplay::MapChars;

	void SpawnNpcHandler::handle(InPacket& recv) const
	{
		int32_t oid = recv.readint();
		int32_t id = recv.readint();
		Point<int16_t> position = recv.readpoint();
		bool flip = recv.readbool();
		uint16_t fh = recv.readshort();

		recv.readshort(); // 'rx'
		recv.readshort(); // 'ry'

		Stage::get().queuespawn(new NpcSpawn(oid, id, position, flip, fh));
	}

	void SpawnNpcControllerHandler::handle(InPacket& recv) const
	{
		int8_t mode = recv.readbyte();
		int32_t oid = recv.readint();

		if (mode == 0)
		{
			Stage::get().getnpcs().removenpc(oid);
		}
		else
		{
			int32_t id = recv.readint();
			Point<int16_t> position = recv.readpoint();
			bool flip = recv.readbool();
			uint16_t fh = recv.readshort();

			recv.readshort(); // 'rx'
			recv.readshort(); // 'ry'
			recv.readbool(); // 'minimap'

			Stage::get().queuespawn(new NpcSpawn(oid, id, position, flip, fh));
		}
	}

	void SpawnMobHandler::handle(InPacket& recv) const
	{
		int32_t oid = recv.readint();
		recv.readbyte(); // 5 if controller == null
		int32_t id = recv.readint();

		recv.skip(22);

		Point<int16_t> position = recv.readpoint();
		int8_t stance = recv.readbyte();

		recv.skip(2);

		uint16_t fh = recv.readshort();
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

		int8_t team = recv.readbyte();

		recv.skip(4);

		Stage::get().queuespawn(new MobSpawn(oid, id, 0, stance, fh, effect == -2, team, position));
	}

	void SpawnMobControllerHandler::handle(InPacket& recv) const
	{
		int8_t mode = recv.readbyte();
		int32_t oid = recv.readint();
		if (mode == 0)
		{
			Stage::get().getmobs().setcontrol(oid, false);
		}
		else
		{
			if (recv.hasmore())
			{
				recv.skip(1);

				int32_t id = recv.readint();

				recv.skip(22);

				Point<int16_t> position = recv.readpoint();
				int8_t stance = recv.readbyte();

				recv.skip(2);

				uint16_t fh = recv.readshort();
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

				int8_t team = recv.readbyte();

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
		int32_t oid = recv.readint();

		recv.readbyte();
		recv.readbyte(); // useskill
		recv.readbyte(); // skill
		recv.readbyte(); // skill 1
		recv.readbyte(); // skill 2
		recv.readbyte(); // skill 3
		recv.readbyte(); // skill 4

		Point<int16_t> position = recv.readpoint();
		queue<Movement> movements = recv.readqueue<uint8_t, Movement>();

		Stage::get().getmobs().movemob(oid, position, movements.front());
	}

	void ShowMobHpHandler::handle(InPacket& recv) const
	{
		int32_t oid = recv.readint();
		int8_t hppercent = recv.readbyte();
		uint16_t playerlevel = Stage::get().getplayer().getstats().getstat(Maplestat::LEVEL);

		Stage::get().getmobs().sendmobhp(oid, hppercent, playerlevel);
	}

	void KillMobHandler::handle(InPacket& recv) const
	{
		int32_t oid = recv.readint();
		int8_t animation = recv.readbyte();

		Stage::get().getmobs().killmob(oid, animation);
	}

	void SpawnCharHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.readint();
		uint8_t level = recv.readbyte();
		string name = recv.read<string>();

		recv.read<string>(); // guildname
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
		LookEntry look = Session::get().getlogin().parselook(recv);

		recv.readint(); //count of 5110000 
		recv.readint(); // 'itemeffect'
		recv.readint(); // 'chair'

		Point<int16_t> position = recv.readpoint();
		int8_t stance = recv.readbyte();

		recv.skip(3);

		for (size_t i = 0; i < 3; i++)
		{
			int8_t available = recv.readbyte();
			if (available == 1)
			{
				recv.readbyte(); // 'byte2'
				recv.readint(); // petid
				recv.read<string>(); // name
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
		string chalktext = chalkboard ? recv.read<string>() : "";

		recv.skip(3);
		recv.readbyte(); // team

		Stage::get().queuespawn(new CharSpawn(cid, look, level, job, name, stance, position));
	}


	void RemoveCharHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.readint();

		Stage::get().getchars().removechar(cid);
	}


	void CharMovedHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.readint();
		recv.skip(4);
		queue<Movement> movements = recv.readqueue<uint8_t, Movement>();

		Stage::get().getchars().movechar(cid, movements);
	}


	void UpdateCharLookHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.readint();
		recv.readbyte();
		LookEntry look = Session::get().getlogin().parselook(recv);

		Stage::get().getchars().updatelook(cid, look);
	}


	void SpawnPetHandler::handle(InPacket& recv) const
	{
		using Character::Char;
		Optional<Char> character = Stage::get().getcharacter(recv.readint());
		if (!character)
			return;

		uint8_t petindex = recv.readbyte();
		int8_t mode = recv.readbyte();

		if (mode == 1)
		{
			recv.skip(1);

			int32_t itemid = recv.readint();
			string name = recv.read<string>();
			int32_t uniqueid = recv.readint();

			recv.skip(4);

			Point<int16_t> pos = recv.readpoint();
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


	void DropItemHandler::handle(InPacket& recv) const
	{
		int8_t mode = recv.readbyte();
		int32_t oid = recv.readint();
		bool meso = recv.readbool();
		int32_t itemid = recv.readint();
		int32_t owner = recv.readint();
		int8_t pickuptype = recv.readbyte();
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
		bool playerdrop = !recv.readbool();

		Stage::get().queuespawn(new DropSpawn(oid, itemid, meso, owner, dropfrom, dropto, pickuptype, mode, playerdrop));
	}


	void RemoveDropHandler::handle(InPacket& recv) const
	{
		int8_t mode = recv.readbyte();
		int32_t oid = recv.readint();

		using Gameplay::PhysicsObject;
		Optional<PhysicsObject> looter;
		if (mode > 1)
		{
			int32_t cid = recv.readint();
			if (recv.length() > 0)
			{
				recv.readbyte(); // pet
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