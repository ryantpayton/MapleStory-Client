/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "InPacket.h"

#include "..\Character\Maplestat.h"
#include "..\Character\Job.h"
#include "..\Util\EnumMap.h"

#include <cstdint>
#include <string>
#include <map>
#include <vector>

namespace jrc
{
	struct StatsEntry
	{
		std::string name;
		std::vector<int64_t> petids;
		EnumMap<Maplestat::Value, uint16_t> stats;
		Job job;
		int64_t exp;
		int32_t mapid;
		uint8_t portal;
		std::pair<int32_t, int8_t> rank;
		std::pair<int32_t, int8_t> jobrank;
	};

	struct LookEntry
	{
		bool female;
		uint8_t skin;
		int32_t faceid;
		int32_t hairid;
		std::map<int8_t, int32_t> equips;
		std::map<int8_t, int32_t> maskedequips;
		std::vector<int32_t> petids;
	};

	struct CharEntry
	{
		StatsEntry stats;
		LookEntry look;
		int32_t cid = 0;

		const StatsEntry& get_stats() const { return stats; }
		const LookEntry& get_look() const { return look; }
	};

	struct Account
	{
		std::vector<CharEntry> chars;
		std::string name = "";
		int32_t accid = 0;
		bool female = true;
		bool muted = false;
		int16_t pin = 0;
		int8_t gmlevel = 0;
		int8_t pic = 0;
		int8_t slots = 0;
		int8_t selected = 0;
	};

	struct World
	{
		uint8_t wid = 0;
		std::string name = "";
		int8_t flag = 0;
		std::string message = "";
		uint8_t channelcount = 0;
		std::vector<int8_t> chloads;
	};

	class Login
	{
	public:
		void parse_account(InPacket& recv);
		void parse_worlds(InPacket& recv);
		void parse_charlist(InPacket& recv);
		void add_charentry(InPacket& recv);
		void remove_char(int32_t cid);

		CharEntry parse_charentry(InPacket& recv) const;
		StatsEntry parse_stats(InPacket& recv) const;
		LookEntry parse_look(InPacket& recv) const;

		const CharEntry& get_char_by_index(size_t) const;
		const CharEntry& find_char_by_id(int32_t) const;

		void set_world(uint8_t w) { worldid = w; }
		void set_channel(uint8_t c) { channelid = c; }
		uint8_t get_world() const { return worldid; }
		uint8_t get_channel() const { return channelid; }
		size_t getnumworlds() const { return worlds.size(); }
		const Account& getaccount() const { return account; }
		const World& getworld(size_t w) const { return worlds[w]; }

	private:
		Account account;
		CharEntry nullchar;
		std::vector<World> worlds;
		uint8_t worldid;
		uint8_t channelid;
	};
}