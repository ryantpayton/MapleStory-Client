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
#include "Net\InPacket.h"
#include <vector>

namespace Net
{
	using::std::vector;

	class World
	{
	public:
		World(uint8_t, InPacket&);
		~World() {}
		uint8_t getid() const;
		uint8_t getchcount() const;
		int8_t getchload(size_t) const;
	private:
		uint8_t wid;
		string name;
		int8_t flag;
		string message;
		uint8_t channelcount;
		vector<int8_t> chloads;
	};
}

