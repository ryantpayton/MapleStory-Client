#pragma once
#include "InPacket.h"

namespace Net
{
	class StatsEntry
	{
	public:
		StatsEntry(InPacket*);
		StatsEntry() {}
		~StatsEntry() {}
		int getid() { return cid; }
		string getname() { return name; }
		vector<int64_t>* getpets() { return &petids; }
		short getlevel() { return level; }
		short getjob() { return job; }
		short getstr() { return str; }
		short getdex() { return dex; }
		short getint() { return int_; }
		int gethp() { return hp; }
		int getmaxhp() { return maxhp; }
		int getmp() { return mp; }
		int getmaxmp() { return maxmp; }
		short getap() { return ap; }
		short getsp() { return sp; }
		int64_t getexp() { return exp; }
		short getfame() { return fame; }
		int getmapid() { return mapid; }
		uint8_t getportal() { return portal; }
	private:
		int cid;
		string name;
		vector<int64_t> petids;
		short level;
		short job;
		short str;
		short dex;
		short int_;
		short luk;
		int hp;
		int maxhp;
		int mp;
		int maxmp;
		short ap;
		short sp;
		int64_t exp;
		short fame;
		int mapid;
		uint8_t portal;
	};
}

