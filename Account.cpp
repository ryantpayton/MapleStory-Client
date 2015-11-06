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
#include "Account.h"

namespace Net
{
	Account::Account(InPacket& recv)
	{
		recv.readshort();
		accid = recv.readint();
		female = recv.readbool();
		recv.readbool(); //is admin
		gmlevel = recv.readbyte();
		recv.readbyte();
		name = recv.readascii();
		recv.readbyte();
		muted = recv.readbool();
		recv.readlong(); //muted until
		recv.readlong(); //creation date
		recv.readint();
		pin = recv.readshort();
		selected = 0;
	}

	void Account::parsecharentry(InPacket& recv)
	{
		CharEntry toadd = CharEntry(recv);
		chars.push_back(toadd);
	}

	void Account::setpic(char p)
	{
		pic = p;
	}

	void Account::setslots(char s)
	{
		slots = s;
	}

	char Account::getpic() const
	{
		return pic;
	}

	char Account::getslots() const
	{
		return slots;
	}

	size_t Account::getcharcount() const
	{
		return chars.size();
	}

	string Account::getname() const
	{
		return name;
	}

	const CharEntry& Account::getchar(size_t i) const 
	{ 
		if (i < chars.size())
		{
			return chars[i];
		}
		else
		{
			return nullchar;
		}
	}

	const CharEntry& Account::getcharbyid(int cid) const
	{
		for (size_t i = 0; i < chars.size(); i++)
		{
			if (chars.at(i).getcid() == cid)
			{
				return chars.at(i);
			}
		}
		return nullchar;
	}
}
