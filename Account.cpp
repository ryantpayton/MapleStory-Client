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
#include "Account.h"

namespace Net
{
	Account::Account(InPacket* recv)
	{
		recv->readshort();
		accid = recv->readint();
		female = recv->readbool();
		recv->readbool(); //is admin
		gmlevel = recv->readbyte();
		recv->readbyte();
		name = recv->readascii();
		recv->readbyte();
		muted = recv->readbool();
		recv->readlong(); //muted until
		recv->readlong(); //creation date
		recv->readint();
		pin = recv->readshort();
	}

	void Account::parsecharentry(InPacket* recv)
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
}
