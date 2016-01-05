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
#include "Gameplay\Stage.h"

#include "Journey.h"

namespace Net
{
	using Gameplay::MapNpcs;
	MapNpcs& getnpcs()
	{
		return Gameplay::Stage::getnpcs();
	}

	using Gameplay::MapMobs;
	MapMobs& getmobs()
	{
		return Gameplay::Stage::getmobs();
	}

	using Gameplay::MapChars;
	MapChars& getchars()
	{
		return Gameplay::Stage::getchars();
	}

	using Gameplay::MapDrops;
	MapDrops& getdrops()
	{
		return Gameplay::Stage::getdrops();
	}

	void SpawnNpcHandler::handle(InPacket& recv) const
	{
		int32_t oid = recv.readint();
		int32_t id = recv.readint();
		vector2d<int16_t> position = recv.readpoint();
		bool flip = recv.readbool();
		uint16_t fh = recv.readshort();

		recv.readshort(); // 'rx'
		recv.readshort(); // 'ry'

		getnpcs().addnpc(id, oid, flip, fh, true, position);
	}

	void SpawnNpcControllerHandler::handle(InPacket& recv) const
	{
		int8_t mode = recv.readbyte();
		int32_t oid = recv.readint();

		if (mode == 0)
		{

		}
		else
		{
			int32_t id = recv.readint();
			vector2d<int16_t> position = recv.readpoint();
			bool f = recv.readbool();
			uint16_t fh = recv.readshort();

			recv.readshort(); // 'rx'
			recv.readshort(); // 'ry'
			recv.readbool(); // 'minimap'

			getnpcs().addnpc(id, oid, f, fh, true, position);
		}
	}

	void SpawnMobHandler::handle(InPacket& recv) const
	{
		int32_t oid = recv.readint();
		bool hascontrol = recv.readbyte() == 5;
		int32_t id = recv.readint();

		recv.skip(22);

		vector2d<int16_t> position = recv.readpoint();
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

		getmobs().addmob(oid, id, hascontrol, stance, fh, effect == -2, team, position);
	}

	void SpawnMobControllerHandler::handle(InPacket& recv) const
	{
		int8_t aggro = recv.readbyte();
		if (aggro == 0)
			return;

		int32_t oid = recv.readint();

		recv.skip(1);

		int32_t id = recv.readint();

		recv.skip(22);

		vector2d<int16_t> position = recv.readpoint();
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

		getmobs().addmob(oid, id, true, stance, fh, effect == -2, team, position);
	}

	void ShowMobHpHandler::handle(InPacket& recv) const
	{
		int32_t oid = recv.readint();
		int8_t hppercent = recv.readbyte();
		uint16_t playerlevel = Gameplay::Stage::getplayer().getstats().getstat(Character::MS_LEVEL);

		getmobs().sendmobhp(oid, hppercent, playerlevel);
	}

	void KillMobHandler::handle(InPacket& recv) const
	{
		int32_t oid = recv.readint();
		int8_t animation = recv.readbyte();

		getmobs().killmob(oid, animation);
	}

	void SpawnCharHandler::handle(InPacket& recv) const
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

		getchars().addchar(cid, look, level, job, name, stance, position);
	}

	void RemoveCharHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.readint();

		getchars().removechar(cid);
	}

	MovementFragment MovementHandler::parsemovement(InPacket& recv) const
	{
		MovementFragment fragment;

		fragment.command = recv.readbyte();
		switch (fragment.command)
		{
		case 0:
		case 5:
		case 17:
			fragment.type = MovementFragment::MVT_ABSOLUTE;
			fragment.xpos = recv.readshort();
			fragment.ypos = recv.readshort();
			fragment.lastx = recv.readshort();
			fragment.lasty = recv.readshort();
			recv.skip(2);
			fragment.newstate = recv.readbyte();
			fragment.duration = recv.readshort();
			break;
		case 1:
		case 2:
		case 6:
		case 12:
		case 13:
		case 16:
			fragment.type = MovementFragment::MVT_RELATIVE;
			fragment.xpos = recv.readshort();
			fragment.ypos = recv.readshort();
			fragment.newstate = recv.readbyte();
			fragment.duration = recv.readshort();
			break;
		case 11:
			fragment.type = MovementFragment::MVT_CHAIR;
			fragment.xpos = recv.readshort();
			fragment.ypos = recv.readshort();
			recv.skip(2);
			fragment.newstate = recv.readbyte();
			fragment.duration = recv.readshort();
			break;
		case 15:
			fragment.type = MovementFragment::MVT_JUMPDOWN;
			fragment.xpos = recv.readshort();
			fragment.ypos = recv.readshort();
			fragment.lastx = recv.readshort();
			fragment.lasty = recv.readshort();
			recv.skip(2);
			fragment.fh = recv.readshort();
			fragment.newstate = recv.readbyte();
			fragment.duration = recv.readshort();
			break;
		case 3:
		case 4:
		case 7:
		case 8:
		case 9:
		case 14:
			fragment.type = MovementFragment::MVT_NONE;
			break;
		case 10:
			fragment.type = MovementFragment::MVT_NONE;
			//change equip
			break;
		}

		return fragment;
	}

	void MoveCharHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.readint();

		recv.skip(4);

		vector<MovementFragment> movements;
		uint8_t nummoves = recv.readbyte();
		for (uint8_t i = 0; i < nummoves; i++)
		{
			movements.push_back(parsemovement(recv));
		}

		getchars().movechar(cid, movements);
	}

	void SpawnPetHandler::handle(InPacket& recv) const
	{
		using Character::Char;
		Char* character = Gameplay::Stage::getcharacter(recv.readint());

		if (character == nullptr)
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

	void DropItemHandler::handle(InPacket& recv) const
	{
		int8_t mode = recv.readbyte();
		int32_t oid = recv.readint();
		bool meso = recv.readbool();
		int32_t itemid = recv.readint();
		int32_t owner = recv.readint();
		int8_t pickuptype = recv.readbyte();
		vector2d<int16_t> dropto = recv.readpoint();

		recv.skip(4);

		vector2d<int16_t> dropfrom;
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

		getdrops().adddrop(oid, itemid, meso, owner, dropfrom, dropto, pickuptype, mode, playerdrop);
	}

	void RemoveDropHandler::handle(InPacket& recv) const
	{
		int8_t anim = recv.readbyte();
		int32_t oid = recv.readint();

		using Gameplay::PhysicsObject;
		const PhysicsObject* looter = nullptr;

		if (anim > 1)
		{
			int32_t cid = recv.readint();
			if (recv.length() > 0)
			{
				recv.readbyte(); // pet
			}
			else
			{
				using Character::Char;
				Char* charlooter = Gameplay::Stage::getcharacter(cid);

				if (charlooter)
					looter = &charlooter->getphobj();
			}
			/*if (cid == Game::getfield()->getplayer()->getoid())
			{
			Game::getcache()->getsounds()->play(MSN_PICKUP);
			}
			looter = Game::getfield()->getchar(cid);*/
		}

		getdrops().removedrop(oid, anim, looter);
	}
}