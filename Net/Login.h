//////////////////////////////////////////////////////////////////////////////
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
#include "../Character/Maplestat.h"
#include "../Template/EnumMap.h"

#include <cstdint>
#include <string>
#include <vector>
#include <map>

namespace jrc
{
	struct Account
	{
		std::string name;
		int32_t accid;
		bool female;
		bool muted;
		int16_t pin;
		int8_t gmlevel;
	};

	struct World
	{
		std::string name;
		std::string message;
		std::vector<int32_t> chloads;
		uint8_t channelcount;
		uint8_t flag;
		int8_t wid;
	};

	struct StatsEntry
	{
		std::string name;
		std::vector<int64_t> petids;
		EnumMap<Maplestat::Id, uint16_t> stats;
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
		int32_t cid;
	};
}