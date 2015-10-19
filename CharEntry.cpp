/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
			int currank = recv.readint();
			int rankmv = recv.readint();
			int curjobrank = recv.readint();
			int jobrankmv = recv.readint();
			char rankmc = (rankmv > 0) ? '+' : (rankmv < 0) ? '-' : '=';
			char jobrankmc = (jobrankmv > 0) ? '+' : (jobrankmv < 0) ? '-' : '=';
			stats.setrank(make_pair(currank, rankmc));
			stats.setjobrank(make_pair(curjobrank, jobrankmc));
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

	const int CharEntry::getcid() const
	{
		return cid;
	}
}
