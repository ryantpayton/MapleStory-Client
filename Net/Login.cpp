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

namespace jrc
{
	void Login::parseaccount(InPacket& recv)
	{
		recv.read_short();
		account.accid = recv.read_int();
		account.female = recv.read_bool();
		recv.read_bool(); //is admin
		account.gmlevel = recv.read_byte();
		recv.read_byte();
		account.name = recv.read_string();
		recv.read_byte();
		account.muted = recv.read_bool();
		recv.read_long(); //muted until
		recv.read_long(); //creation date
		recv.read_int();
		account.pin = recv.read_short();
		account.selected = 0;
	}

	void Login::parseworlds(InPacket& recv)
	{
		int8_t wid = recv.read_byte();
		if (wid != -1)
		{
			World world;
			world.wid = wid;
			world.name = recv.read_string();
			world.flag = recv.read_byte();
			world.message = recv.read_string();

			recv.skip(5);

			world.channelcount = recv.read_byte();
			for (uint8_t i = 0; i < world.channelcount; i++)
			{
				recv.read_string();
				recv.skip(4);
				world.chloads.push_back(recv.read_byte());
				recv.skip(2);
			}

			worlds.push_back(world);
		}
	}

	void Login::parsecharlist(InPacket& recv)
	{
		size_t numchars = recv.read_byte();
		for (size_t i = 0; i < numchars; i++)
		{
			addcharentry(recv);
		}

		account.pic = recv.read_byte();
		account.slots = static_cast<int8_t>(recv.read_int());
	}

	void Login::addcharentry(InPacket& recv)
	{
		account.chars.push_back(parsecharentry(recv));
	}

	CharEntry Login::parsecharentry(InPacket& recv) const
	{
		CharEntry toparse;
		toparse.cid = recv.read_int();
		toparse.stats = parsestats(recv);
		toparse.look = parselook(recv);

		recv.read_bool(); // 'rankinfo' bool
		if (recv.read_bool())
		{
			int32_t currank = recv.read_int();
			int32_t rankmv = recv.read_int();
			int32_t curjobrank = recv.read_int();
			int32_t jobrankmv = recv.read_int();
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

		look.female = recv.read_bool();
		look.skin = recv.read_byte();
		look.faceid = recv.read_int();
		recv.read_bool(); //megaphone
		look.hairid = recv.read_int();
		uint8_t slot = recv.read_byte();
		while (slot != 0xFF)
		{
			look.equips[slot] = recv.read_int();
			slot = recv.read_byte();
		}
		slot = recv.read_byte();
		while (slot != 0xFF)
		{
			look.maskedequips[slot] = recv.read_int();
			slot = recv.read_byte();
		}
		look.maskedequips[-111] = recv.read_int();
		for (uint8_t i = 0; i < 3; i++)
		{
			look.petids.push_back(recv.read_int());
		}

		return look;
	}

	StatsEntry Login::parsestats(InPacket& recv) const
	{
		StatsEntry statsentry;

		statsentry.name = recv.read_padded_string(13);

		recv.read_bool(); //gender
		recv.read_byte(); //skin
		recv.read_int(); //face
		recv.read_int(); //hair

		for (size_t i = 0; i < 3; i++)
		{
			statsentry.petids.push_back(recv.read_long());
		}

		statsentry.stats[Maplestat::LEVEL] = recv.read_byte();
		statsentry.stats[Maplestat::JOB] = recv.read_short();
		statsentry.stats[Maplestat::STR] = recv.read_short();
		statsentry.stats[Maplestat::DEX] = recv.read_short();
		statsentry.stats[Maplestat::INT] = recv.read_short();
		statsentry.stats[Maplestat::LUK] = recv.read_short();
		statsentry.stats[Maplestat::HP] = recv.read_short();
		statsentry.stats[Maplestat::MAXHP] = recv.read_short();
		statsentry.stats[Maplestat::MP] = recv.read_short();
		statsentry.stats[Maplestat::MAXMP] = recv.read_short();
		statsentry.stats[Maplestat::AP] = recv.read_short();
		statsentry.stats[Maplestat::SP] = recv.read_short();
		statsentry.exp = recv.read_int();
		statsentry.stats[Maplestat::FAME] = recv.read_short();

		recv.skip(4); //gachaexp
		statsentry.mapid = recv.read_int();
		statsentry.portal = recv.read_byte();
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