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
#include "CharEntry.h"

namespace Net
{
	CharEntry::CharEntry(InPacket& recv)
	{
		cid = recv.readint();
		stats = StatsEntry(recv);
		look = LookEntry(recv);
		bool rankinfo = recv.readbool();
		bool notgm = recv.readbool();
		if (notgm)
		{
			int32_t currank = recv.readint();
			int32_t rankmv = recv.readint();
			int32_t curjobrank = recv.readint();
			int32_t jobrankmv = recv.readint();
			int8_t rankmc = (rankmv > 0) ? '+' : (rankmv < 0) ? '-' : '=';
			int8_t jobrankmc = (jobrankmv > 0) ? '+' : (jobrankmv < 0) ? '-' : '=';
			stats.setrank(std::make_pair(currank, rankmc));
			stats.setjobrank(std::make_pair(curjobrank, jobrankmc));
		}
	}

	CharEntry::CharEntry()
	{
		cid = 0;
	}

	const StatsEntry& CharEntry::getstats() const
	{
		return stats;
	}

	const LookEntry& CharEntry::getlook() const
	{
		return look;
	}

	const int32_t CharEntry::getcid() const
	{
		return cid;
	}
}
