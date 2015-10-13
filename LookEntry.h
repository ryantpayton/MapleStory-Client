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
#pragma once
#include "InPacket.h"
#include <map>

namespace Net
{
	class LookEntry
	{
	public:
		LookEntry(InPacket*);
		LookEntry(bool, uint8_t, int, int, map<uint8_t, int>);
		LookEntry() {}
		~LookEntry() {}
		bool isfemale();
		uint8_t getskin();
		int getface();
		int gethair();
		map<uint8_t, int>* getequips();
		map<uint8_t, int>* getmaskedequips();
		vector<int>* getpets();
	private:
		bool female;
		uint8_t skin;
		int faceid;
		int hairid;
		map<uint8_t, int> equips;
		map<uint8_t, int> maskedequips;
		vector<int> petids;
	};
}

