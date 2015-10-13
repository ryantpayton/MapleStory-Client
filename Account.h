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
#include "CharEntry.h"

namespace Net
{
	class Account
	{
	public:
		Account(InPacket*);
		Account() {}
		~Account() {}
		void parsecharentry(InPacket*);
		void setpic(char);
		void setslots(char);
		char getpic() { return pic; }
		char getslots() { return slots; }
		vector<CharEntry>* getchars() { return &chars; }
	private:
		vector<CharEntry> chars;
		string name;
		int accid;
		bool female;
		bool muted;
		short pin;
		char gmlevel;
		char pic;
		char slots;
		uint8_t selection;
	};
}

