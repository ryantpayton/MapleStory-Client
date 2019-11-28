//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "LoginParser.h"

namespace ms
{
	Account LoginParser::parse_account(InPacket& recv)
	{
		Account account;

		recv.skip(2);

		account.accid = recv.read_int();
		account.female = recv.read_byte();

		recv.read_bool(); // is admin

		account.gmlevel = recv.read_byte();

		recv.skip(1);

		account.name = recv.read_string();

		recv.skip(1);

		account.muted = recv.read_bool();

		recv.read_long(); // muted until
		recv.read_long(); // creation date
		recv.skip(4);

		account.pin = recv.read_short();

		return account;
	}

	World LoginParser::parse_world(InPacket& recv)
	{
		int8_t wid = recv.read_byte();

		if (wid == -1)
			return { {}, {}, {}, 0, 0, wid };

		std::string name = recv.read_string();
		uint8_t flag = recv.read_byte();
		std::string message = recv.read_string();

		recv.skip(5);

		std::vector<int32_t> chloads;
		uint8_t channelcount = recv.read_byte();

		for (uint8_t i = 0; i < channelcount; ++i)
		{
			recv.read_string(); // channel name

			chloads.push_back(recv.read_int());

			recv.skip(1);
			recv.skip(2);
		}

		recv.skip(2);

		return { name, message, chloads, channelcount, flag, wid };
	}

	RecommendedWorld LoginParser::parse_recommended_world(InPacket& recv)
	{
		int32_t wid = recv.read_int();

		if (wid == -1)
			return { {}, wid };

		std::string message = recv.read_string();

		return { message, wid };
	}

	CharEntry LoginParser::parse_charentry(InPacket& recv)
	{
		int32_t cid = recv.read_int();
		StatsEntry stats = parse_stats(recv);
		LookEntry look = parse_look(recv);

		recv.read_bool(); // 'rankinfo' bool

		if (recv.read_bool())
		{
			int32_t currank = recv.read_int();
			int32_t rankmv = recv.read_int();
			int32_t curjobrank = recv.read_int();
			int32_t jobrankmv = recv.read_int();
			int8_t rankmc = (rankmv > 0) ? '+' : (rankmv < 0) ? '-' : '=';
			int8_t jobrankmc = (jobrankmv > 0) ? '+' : (jobrankmv < 0) ? '-' : '=';

			stats.rank = std::make_pair(currank, rankmc);
			stats.jobrank = std::make_pair(curjobrank, jobrankmc);
		}

		return { stats, look, cid };
	}

	StatsEntry LoginParser::parse_stats(InPacket& recv)
	{
		StatsEntry statsentry;

		statsentry.name = recv.read_padded_string(13);
		statsentry.female = recv.read_bool();

		recv.read_byte();	// skin
		recv.read_int();	// face
		recv.read_int();	// hair

		for (size_t i = 0; i < 3; i++)
			statsentry.petids.push_back(recv.read_long());

		statsentry.stats[Maplestat::Id::LEVEL] = recv.read_byte(); // TODO: Change to recv.read_short(); to increase level cap
		statsentry.stats[Maplestat::Id::JOB] = recv.read_short();
		statsentry.stats[Maplestat::Id::STR] = recv.read_short();
		statsentry.stats[Maplestat::Id::DEX] = recv.read_short();
		statsentry.stats[Maplestat::Id::INT] = recv.read_short();
		statsentry.stats[Maplestat::Id::LUK] = recv.read_short();
		statsentry.stats[Maplestat::Id::HP] = recv.read_short();
		statsentry.stats[Maplestat::Id::MAXHP] = recv.read_short();
		statsentry.stats[Maplestat::Id::MP] = recv.read_short();
		statsentry.stats[Maplestat::Id::MAXMP] = recv.read_short();
		statsentry.stats[Maplestat::Id::AP] = recv.read_short();
		statsentry.stats[Maplestat::Id::SP] = recv.read_short();
		statsentry.exp = recv.read_int();
		statsentry.stats[Maplestat::Id::FAME] = recv.read_short();

		recv.skip(4); // gachaexp

		statsentry.mapid = recv.read_int();
		statsentry.portal = recv.read_byte();

		recv.skip(4); // timestamp

		return statsentry;
	}

	LookEntry LoginParser::parse_look(InPacket& recv)
	{
		LookEntry look;

		look.female = recv.read_bool();
		look.skin = recv.read_byte();
		look.faceid = recv.read_int();

		recv.read_bool(); // megaphone

		look.hairid = recv.read_int();

		uint8_t eqslot = recv.read_byte();

		while (eqslot != 0xFF)
		{
			look.equips[eqslot] = recv.read_int();
			eqslot = recv.read_byte();
		}

		uint8_t mskeqslot = recv.read_byte();

		while (mskeqslot != 0xFF)
		{
			look.maskedequips[mskeqslot] = recv.read_int();
			mskeqslot = recv.read_byte();
		}

		look.maskedequips[-111] = recv.read_int();

		for (uint8_t i = 0; i < 3; i++)
			look.petids.push_back(recv.read_int());

		return look;
	}
}