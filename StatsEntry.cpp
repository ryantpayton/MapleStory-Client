#include "StatsEntry.h"

namespace Net
{
	StatsEntry::StatsEntry(InPacket* recv)
	{
		cid = recv->readint();
		name = recv->readpadascii(13);
		recv->readbool(); //gender
		recv->readbyte(); //skin
		recv->readint(); //face
		recv->readint(); //hair
		for (uint8_t i = 0; i < 3; i++)
		{
			petids.push_back(recv->readlong());
		}
		level = static_cast<uint8_t>(recv->readbyte());
		job = recv->readshort();
		str = recv->readshort();
		dex = recv->readshort();
		int_ = recv->readshort();
		luk = recv->readshort();
		hp = recv->readshort();
		maxhp = recv->readshort();
		mp = recv->readshort();
		maxmp = recv->readshort();
		ap = recv->readshort();
		sp = recv->readshort();
		exp = recv->readint();
		fame = recv->readshort();
		recv->readint(); //gachaexp
		mapid = recv->readint();
		portal = recv->readbyte();
		recv->readint(); //timestamp
	}
}
