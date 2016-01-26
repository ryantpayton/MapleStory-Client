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
#include "Login.h"

namespace Net
{
	Login::Login() {}

	Login::~Login() {}

	void Login::parseaccount(InPacket& recv)
	{
		recv.readshort();
		account.accid = recv.readint();
		account.female = recv.readbool();
		recv.readbool(); //is admin
		account.gmlevel = recv.readbyte();
		recv.readbyte();
		account.name = recv.readascii();
		recv.readbyte();
		account.muted = recv.readbool();
		recv.readlong(); //muted until
		recv.readlong(); //creation date
		recv.readint();
		account.pin = recv.readshort();
		account.selected = 0;
	}

	void Login::parseworlds(InPacket& recv)
	{
		int8_t worldid = recv.readbyte();
		if (worldid != -1)
		{
			World world;
			world.wid = worldid;
			world.name = recv.readascii();
			world.flag = recv.readbyte();
			world.message = recv.readascii();

			recv.skip(5);

			world.channelcount = recv.readbyte();
			for (uint8_t i = 0; i < world.channelcount; i++)
			{
				recv.readascii();
				recv.skip(4);
				world.chloads.push_back(recv.readbyte());
				recv.skip(2);
			}

			worlds.push_back(world);
		}
	}

	void Login::parsecharlist(InPacket& recv)
	{
		size_t numchars = recv.readbyte();
		for (size_t i = 0; i < numchars; i++)
		{
			addcharentry(recv);
		}

		account.pic = recv.readbyte();
		account.slots = static_cast<int8_t>(recv.readint());
	}

	void Login::addcharentry(InPacket& recv)
	{
		account.chars.push_back(parsecharentry(recv));
	}

	CharEntry Login::parsecharentry(InPacket& recv) const
	{
		CharEntry toparse;
		toparse.cid = recv.readint();
		toparse.stats = parsestats(recv);
		toparse.look = parselook(recv);

		recv.readbool(); // 'rankinfo' bool
		if (recv.readbool())
		{
			int32_t currank = recv.readint();
			int32_t rankmv = recv.readint();
			int32_t curjobrank = recv.readint();
			int32_t jobrankmv = recv.readint();
			int8_t rankmc = (rankmv > 0) ? '+' : (rankmv < 0) ? '-' : '=';
			int8_t jobrankmc = (jobrankmv > 0) ? '+' : (jobrankmv < 0) ? '-' : '=';

			toparse.stats.rank = std::make_pair(currank, rankmc);
			toparse.stats.jobrank = std::make_pair(curjobrank, jobrankmc);
		}

		return toparse;
	}

	LookEntry Login::parselook(InPacket& recv) const
	{
		LookEntry look;

		look.female = recv.readbool();
		look.skin = recv.readbyte();
		look.faceid = recv.readint();
		recv.readbool(); //megaphone
		look.hairid = recv.readint();
		uint8_t slot = recv.readbyte();
		while (slot != 0xFF)
		{
			look.equips[slot] = recv.readint();
			slot = recv.readbyte();
		}
		slot = recv.readbyte();
		while (slot != 0xFF)
		{
			look.maskedequips[slot] = recv.readint();
			slot = recv.readbyte();
		}
		look.maskedequips[-111] = recv.readint();
		for (uint8_t i = 0; i < 3; i++)
		{
			look.petids.push_back(recv.readint());
		}

		return look;
	}

	StatsEntry Login::parsestats(InPacket& recv) const
	{
		StatsEntry statsentry;

		statsentry.name = recv.readpadascii(13);

		recv.readbool(); //gender
		recv.readbyte(); //skin
		recv.readint(); //face
		recv.readint(); //hair

		for (size_t i = 0; i < 3; i++)
		{
			statsentry.petids.push_back(recv.readlong());
		}

		statsentry.stats[Maplestat::LEVEL] = recv.readbyte();
		statsentry.stats[Maplestat::JOB] = recv.readshort();
		statsentry.stats[Maplestat::STR] = recv.readshort();
		statsentry.stats[Maplestat::DEX] = recv.readshort();
		statsentry.stats[Maplestat::INT] = recv.readshort();
		statsentry.stats[Maplestat::LUK] = recv.readshort();
		statsentry.stats[Maplestat::HP] = recv.readshort();
		statsentry.stats[Maplestat::MAXHP] = recv.readshort();
		statsentry.stats[Maplestat::MP] = recv.readshort();
		statsentry.stats[Maplestat::MAXMP] = recv.readshort();
		statsentry.stats[Maplestat::AP] = recv.readshort();
		statsentry.stats[Maplestat::SP] = recv.readshort();
		statsentry.exp = recv.readint();
		statsentry.stats[Maplestat::FAME] = recv.readshort();

		recv.skip(4); //gachaexp
		statsentry.mapid = recv.readint();
		statsentry.portal = recv.readbyte();
		recv.skip(4); //timestamp

		statsentry.job = CharJob(statsentry.stats[Maplestat::JOB]);

		return statsentry;
	}

	const CharEntry& Login::getchar(size_t i) const
	{
		if (i < account.chars.size())
		{
			return account.chars[i];
		}
		else
		{
			return nullchar;
		}
	}

	const CharEntry& Login::getcharbyid(int cid) const
	{
		for (size_t i = 0; i < account.chars.size(); i++)
		{
			if (account.chars.at(i).cid == cid)
			{
				return account.chars.at(i);
			}
		}
		return nullchar;
	}
}