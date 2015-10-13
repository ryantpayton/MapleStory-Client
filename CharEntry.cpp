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
	CharEntry::CharEntry(InPacket* recv)
	{
		stats = StatsEntry(recv);
		look = LookEntry(recv);
		bool rankinfo = recv->readbool();
		bool notgm = recv->readbool();
		if (notgm)
		{
			int currank = recv->readint();
			int rankmv = recv->readint();
			int curjobrank = recv->readint();
			int jobrankmv = recv->readint();
			char rankmc = (rankmv > 0) ? '+' : (rankmv < 0) ? '-' : '=';
			char jobrankmc = (jobrankmv > 0) ? '+' : (jobrankmv < 0) ? '-' : '=';
			rank = make_pair(currank, rankmc);
			jobrank = make_pair(curjobrank, jobrankmc);
		}
	}

	StatsEntry* CharEntry::getstats()
	{
		return &stats;
	}

	LookEntry* CharEntry::getlook()
	{
		return &look;
	}

	pair<int, char> CharEntry::getrank()
	{
		return rank;
	}

	pair<int, char> CharEntry::getjobrank()
	{
		return jobrank;
	}
}
