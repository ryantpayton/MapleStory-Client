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
#include "CharJob.h"

namespace Character
{
	CharJob::CharJob(uint16_t i)
	{
		changejob(i);
	}

	CharJob::CharJob()
	{
		changejob(0);
	}

	CharJob::~CharJob() {}

	void CharJob::changejob(uint16_t i)
	{
		id = i;
		name = getname(id);

		if (id == 0)
		{
			level = JOB_BEGINNER;
		}
		else if (id % 100 == 0)
		{
			level = JOB_FIRST;
		}
		else if (id % 10 == 0)
		{
			level = JOB_SECOND;
		}
		else if (id % 10 == 1)
		{
			level = JOB_THIRD;
		}
		else
		{
			level = JOB_FOURTHT;
		}
	}

	uint16_t CharJob::getid() const
	{
		return id;
	}

	uint16_t CharJob::getsubjob(Joblevel lv) const
	{
		if (lv <= level)
		{
			switch (lv)
			{
			case JOB_BEGINNER:
				return 0;
			case JOB_FIRST:
				return (id / 100) * 100;
			case JOB_SECOND:
				return (id / 10) * 10;
			case JOB_THIRD:
				return (level == JOB_FOURTHT) ? id - 1 : id;
			case JOB_FOURTHT:
				return id;
			}
		}
		return 0;
	}

	string CharJob::getname() const
	{
		return name;
	}

	Joblevel CharJob::getlevel() const
	{
		return level;
	}

	string CharJob::getname(uint16_t id) const
	{
		switch (id)
		{
		case 0:
			return "Beginner";
		case 100:
			return "Swordsman";
		case 110:
			return "Fighter";
		case 111:
			return "Crusader";
		case 112:
			return "Hero";
		case 120:
			return "Page";
		case 121:
			return "White Knight";
		case 122:
			return "Paladin";
		case 130:
			return "Spearman";
		case 131:
			return "Dragon Knight";
		case 132:
			return "Dark Knight";
		case 200:
			return "Magician";
		case 210:
			return "Wizard (F/P)";
		case 211:
			return "Mage (F/P)";
		case 212:
			return "Archmage (F/P)";
		case 220:
			return "Wizard (I/L)";
		case 221:
			return "Mage (I/L)";
		case 222:
			return "Archmage (I/L)";
		case 230:
			return "Cleric";
		case 231:
			return "Priest";
		case 232:
			return "Bishop";
		case 300:
			return "Archer";
		case 310:
			return "Hunter";
		case 311:
			return "Ranger";
		case 312:
			return "Bowmaster";
		case 320:
			return "Crossbowman";
		case 321:
			return "Sniper";
		case 322:
			return "Marksman";
		case 400:
			return "Rogue";
		case 410:
			return "Assassin";
		case 411:
			return "Hermit";
		case 412:
			return "Nightlord";
		case 420:
			return "Bandit";
		case 421:
			return "Chief Bandit";
		case 422:
			return "Shadower";
		case 500:
			return "Pirate";
		case 510:
			return "Brawler";
		case 511:
			return "Marauder";
		case 512:
			return "Buccaneer";
		case 520:
			return "Gunslinger";
		case 521:
			return "Outlaw";
		case 522:
			return "Corsair";
		case 2000:
		case 2100:
		case 2110:
		case 2111:
		case 2112:
			return "Aran";
		}

		return "";
	}
}
