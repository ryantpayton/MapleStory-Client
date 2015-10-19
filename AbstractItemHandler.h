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
#include "ChildHandler.h"

namespace Net
{
	class AbstractItemHandler : public ChildHandler
	{
	protected:
		Item* parseitem(Datacache& cache, InPacket& recv, InventoryType inv)
		{
			char type = recv.readbyte();
			int iid = recv.readint();
			const ItemData& idata = cache.getitem(iid);
			Item* toadd = 0;
			if (inv == IVT_EQUIP || inv == IVT_EQUIPPED)
			{
				toadd = getequip(recv, idata, iid);
			}
			else if (iid >= 5000000 && iid <= 5000102)
			{
				toadd = getpet(recv, idata, iid);
			}
			else
			{
				toadd = getitem(recv, idata, iid);
			}
			if (idata.isloaded())
			{
				return toadd;
			}
			else
			{
				delete toadd;
				return 0;
			}
		}
	private:
		Item* getitem(InPacket& recv, const ItemData& idata, int id)
		{
			bool cash = recv.readbool();
			int64_t uniqueid = cash ? recv.readlong() : -1;
			int64_t expire = recv.readlong();
			short count = recv.readshort();
			string owner = recv.readascii();
			short flag = recv.readshort();
			if ((id / 10000 == 233) || (id / 10000 == 207)) //rechargable
			{
				recv.skip(8);
			}
			return new Item(idata, id, cash, uniqueid, expire, count, owner, flag);
		}

		Pet* getpet(InPacket& recv, const ItemData& idata, int id)
		{
			bool cash = recv.readbool();
			int64_t uniqueid = cash ? recv.readlong() : -1;
			int64_t expire = recv.readlong();
			string petname = recv.readpadascii(13);
			char petlevel = recv.readbyte();
			short closeness = recv.readshort();
			char fullness = recv.readbyte();
			recv.skip(18);
			return new Pet(idata, id, cash, uniqueid, expire, petname, petlevel, closeness, fullness);
		}

		Equip* getequip(InPacket& recv, const ItemData& idata, int id)
		{
			bool cash = recv.readbool();
			int64_t uniqueid = -1;
			if (cash)
			{
				uniqueid = recv.readlong();
			}
			int64_t expire = recv.readlong();
			char slots = recv.readbyte();
			char level = recv.readbyte();
			map<Equipstat, short> stats;
			for (Equipstat e = ES_STR; e <= ES_JUMP; e = (Equipstat)(e + 1))
			{
				stats[e] = recv.readshort();
			}
			string owner = recv.readascii();
			short flag = recv.readshort();
			char itemlevel = 0;
			short itemexp = 0;
			int vicious = 0;
			if (cash)
			{
				recv.skip(10);
			}
			else
			{
				recv.readbyte();
				itemlevel = recv.readbyte();
				recv.readshort();
				itemexp = recv.readshort();
				vicious = recv.readint();
				recv.readlong();
			}
			recv.skip(12);
			return new Equip(idata, id, cash, uniqueid, expire, slots, level, stats, owner, flag, itemlevel, itemexp, vicious);
		}
	};
}