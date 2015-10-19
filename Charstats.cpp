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
#include "Charstats.h"

namespace Character
{
	Charstats::Charstats(const StatsEntry& entry)
	{
		name = entry.getname();
		petids = entry.getpets();
		stats = entry.getstats();
		job = CharJob(entry.getjob());
		exp = entry.getexp();
		mapid = entry.getmapid();
		portal = entry.getportal();
	}

	Charstats::Charstats() {}

	int Charstats::getmapid()
	{
		return mapid;
	}

	uint8_t Charstats::getportal()
	{
		return portal;
	}
}
