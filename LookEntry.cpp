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
#include "LookEntry.h"

namespace Net
{
	LookEntry::LookEntry(InPacket* recv)
	{
		female = recv->readbool();
		skin = recv->readbyte();
		faceid = recv->readint();
		recv->readbool(); //megaphone
		hairid = recv->readint();
		uint8_t slot = recv->readbyte();
		while (slot != 0xFF)
		{
			equips[slot] = recv->readint();
			slot = recv->readbyte();
		}
		slot = recv->readbyte();
		while (slot != 0xFF)
		{
			maskedequips[slot] = recv->readint();
			slot = recv->readbyte();
		}
		maskedequips[-111] = recv->readint();
		for (uint8_t i = 0; i < 3; i++)
		{
			petids.push_back(recv->readint());
		}
	}

	LookEntry::LookEntry(bool fe, uint8_t sk, int fc, int hr, map<uint8_t, int> eq)
	{
		female = fe;
		skin = sk;
		faceid = fc;
		hairid = hr;
		equips = eq;
	}

	bool LookEntry::isfemale()
	{
		return female;
	}

	uint8_t LookEntry::getskin()
	{
		return skin;
	}

	int LookEntry::gethair()
	{
		return hairid;
	}

	int LookEntry::getface()
	{
		return faceid;
	}

	map<uint8_t, int>* LookEntry::getequips()
	{
		return &equips;
	}

	map<uint8_t, int>* LookEntry::getmaskedequips()
	{
		return &maskedequips;
	}

	vector<int>* LookEntry::getpets()
	{
		return &petids;
	}
}
