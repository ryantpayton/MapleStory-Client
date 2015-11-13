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
#include "Charstats.h"

namespace Character
{
	const int64_t exptable[201] = 
	{ 
		1, 15, 34, 57, 92, 135, 372, 560, 840, 1242, 1144, 1573, 2144, 2800, 3640, 4700, 5893, 7360, 9144, 
		11120, 13477, 16268, 19320, 22880, 27008, 31477, 36600, 42444, 48720, 55813, 63800, 86784, 98208, 
		110932, 124432, 139372, 155865, 173280, 192400, 213345, 235372, 259392, 285532, 312928, 342624, 
		374760, 408336, 445544, 483532, 524160, 567772, 598886, 631704, 666321, 702836, 741351, 781976, 
		824828, 870028, 917625, 967995, 1021041, 1076994, 1136013, 1198266, 1263930, 1333194, 1406252, 
		1483314, 1564600, 1650340, 1740778, 1836173, 1936794, 2042930, 2154882, 2272970, 2397528, 2528912, 
		2667496, 2813674, 2967863, 3130502, 3302053, 3483005, 3673873, 3875201, 4087562, 4311559, 4547832, 
		4797053, 5059931, 5337215, 5629694, 5938202, 6263614, 6606860, 6968915, 7350811, 7753635, 8178534, 
		8626718, 9099462, 9598112, 10124088, 10678888, 11264090, 11881362, 12532461, 13219239, 13943653, 
		14707765, 15513750, 16363902, 17260644, 18206527, 19204245, 20256637, 21366700, 22537594, 23772654, 
		25075395, 26449526, 27898960, 29427822, 31040466, 32741483, 34535716, 36428273, 38424542, 40530206, 
		42751262, 45094030, 47565183, 50171755, 52921167, 55821246, 58880250, 62106888, 65510344, 69100311, 
		72887008, 76881216, 81094306, 85594273, 90225770, 95170142, 100385466, 105886589, 111689174, 117809740, 
		124265714, 131075474, 138258410, 145834970, 153826726, 162256430, 171148082, 180526997, 190419876, 
		200854885, 211861732, 223471711, 223471711, 248635353, 262260570, 276632449, 291791906, 307782102, 
		324648562, 342439302, 361204976, 380999008, 401877754, 423900654, 447130410, 471633156, 497478653, 
		524740482, 553496261, 583827855, 615821622, 649568646, 685165008, 722712050, 762316670, 804091623, 
		848155844, 894634784, 943660770, 995373379, 1049919840, 1107455447, 1168144006, 1232158297, 
		1299680571, 1370903066, 1446028554, 1525246918, 1608855764, 1697021059 
	};

	Charstats::Charstats(const StatsEntry& entry)
	{
		name = entry.getname();
		petids = entry.getpets();
		stats = entry.getstats();
		job = CharJob(entry.getjob());
		exp = entry.getexp();
		mapid = entry.getmapid();
		portal = entry.getportal();

		maxdamage = 0;
		mindamage = 0;
		attack = 0;
		honor = 0;
		mastery = 0.5f;
		critical = 0.05f;
		mincrit = 0.5f;
		maxcrit = 0.75f;
		bossdmg = 0.0f;
		ignoredef = 0.0f;
		stance = 0.0f;
		resiststatus = 0.0f;
	}

	Charstats::Charstats() {}

	Charstats::~Charstats() {}

	void Charstats::calculatedamage(Weapon::WpType wtype)
	{
		uint16_t primary = getprimary(wtype);
		uint16_t secondary = getsecondary(wtype);
		maxdamage = static_cast<int32_t>(
			(primary + secondary) * static_cast<float>(attack) / 100
			);
		mindamage = static_cast<int32_t>(
			((primary * 0.9 * mastery) + secondary) * static_cast<float>(attack) / 100
			);
	}

	uint16_t Charstats::getprimary(Weapon::WpType wtype)
	{
		if (wtype != Weapon::WEP_NONE)
		{
			switch (getstat(MS_JOB) / 100)
			{
			case 0:
			case 1:
			case 20:
			case 21:
				return gettotal(ES_STR);
			case 2:
				return gettotal(ES_INT);
			case 3:
				return gettotal(ES_DEX);
			case 4:
				return gettotal(ES_LUK);
			case 5:
				if (wtype == Weapon::WEP_GUN)
					return gettotal(ES_DEX);
				else
					return gettotal(ES_STR);
			}
		}

		return 0;
	}

	uint16_t Charstats::getsecondary(Weapon::WpType wtype)
	{
		if (wtype != Weapon::WEP_NONE)
		{
			switch (getstat(MS_JOB) / 100)
			{
			case 0:
			case 1:
			case 20:
			case 21:
				return gettotal(ES_DEX);
			case 2:
				return gettotal(ES_LUK);
			case 3:
				return gettotal(ES_STR);
			case 4:
				return gettotal(ES_DEX);
			case 5:
				if (wtype == Weapon::WEP_GUN)
					return gettotal(ES_STR);
				else
					return gettotal(ES_DEX);
			}
		}

		return 0;
	}

	void Charstats::setstat(Maplestat stat, uint16_t value)
	{
		stats[stat] = value;
	}

	void Charstats::settotal(Equipstat stat, int32_t value)
	{
		totalstats[stat] = value;
	}

	uint16_t Charstats::calculateaccuracy() const
	{
		float dexacc = static_cast<float>(getstat(MS_DEX)) * 0.8f;
		float lukacc = static_cast<float>(getstat(MS_LUK)) * 0.5f;
		return static_cast<uint16_t>(dexacc + lukacc);
	}

	int64_t Charstats::getexpneeded() const
	{
		return exptable[getstat(MS_LEVEL)];
	}

	uint16_t Charstats::getstat(Maplestat stat) const
	{
		return stats.count(stat) ? stats.at(stat) : 0;
	}

	int32_t Charstats::gettotal(Equipstat stat) const
	{
		return totalstats.count(stat) ? totalstats.at(stat) : 0;
	}
}
