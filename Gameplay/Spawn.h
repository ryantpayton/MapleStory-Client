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
#pragma once
#include "Maplemap\MapObject.h"
#include "Maplemap\Npc.h"
#include "Maplemap\Mob.h"
#include "Maplemap\ItemDrop.h"
#include "Maplemap\MesoDrop.h"

#include "..\Character\OtherChar.h"
#include "..\Net\Login.h"

#include <cstdint>

namespace jrc
{
	class Spawn
	{
	public:
		enum Type
		{
			NPC,
			MOB,
			REACTOR,
			DROP,
			CHARACTER
		};

		virtual ~Spawn() {}

		virtual Type type() const = 0;
	};


	class NpcSpawn : public Spawn
	{
	public:
		NpcSpawn(int32_t o, int32_t i, Point<int16_t> p, bool fl, uint16_t f)
			: oid(o), id(i), position(p), flip(fl), fh(f) {}

		int32_t getoid() const
		{
			return oid;
		}

		Type type() const override
		{
			return NPC;
		}

		Npc* instantiate(const Physics& physics) const
		{
			auto spawnposition = physics.getgroundbelow(position);
			return new Npc(id, oid, flip, fh, false, spawnposition);
		}

	private:
		int32_t oid;
		int32_t id;
		Point<int16_t> position;
		bool flip;
		uint16_t fh;
	};


	class MobSpawn : public Spawn
	{
	public:
		MobSpawn(int32_t o, int32_t i, int8_t m, int8_t st, uint16_t f, bool ns, int8_t t, Point<int16_t> p)
			: oid(o), id(i), mode(m), stance(st), fh(f), newspawn(ns), team(t), position(p) {}

		int8_t getmode() const
		{
			return mode;
		}

		int32_t getoid() const
		{
			return oid;
		}

		Type type() const override
		{
			return MOB;
		}

		Mob* instantiate() const
		{
			return new Mob(oid, id, mode, stance, fh, newspawn, team, position);
		}

	private:
		int32_t oid;
		int32_t id;
		int8_t mode;
		int8_t stance;
		uint16_t fh;
		bool newspawn;
		int8_t team;
		Point<int16_t> position;
	};


	class DropSpawn : public Spawn
	{
	public:
		DropSpawn(int32_t o, int32_t i, bool ms, int32_t ow, Point<int16_t> p, Point<int16_t> d, int8_t t, int8_t m, bool pd)
			: oid(o), id(i), meso(ms), owner(ow), start(p), dest(d), droptype(t), mode(m), playerdrop(pd) {}

		bool ismeso() const
		{
			return meso;
		}

		int32_t getitemid() const
		{
			return id;
		}

		int32_t getoid() const
		{
			return oid;
		}

		Type type() const override
		{
			return DROP;
		}

		MesoDrop* instantiate(const Animation* icon) const
		{
			return new MesoDrop(oid, owner, start, dest, droptype, mode, playerdrop, icon);
		}

		ItemDrop* instantiate(const Texture* icon) const
		{
			return new ItemDrop(oid, owner, start, dest, droptype, mode, id, playerdrop, icon);
		}

	private:
		int32_t oid;
		int32_t id;
		bool meso;
		int32_t owner;
		Point<int16_t> start;
		Point<int16_t> dest;
		int8_t droptype;
		int8_t mode;
		bool playerdrop;
	};


	class CharSpawn : public Spawn
	{
	public:
		CharSpawn(int32_t c, LookEntry lk, uint8_t l, int16_t j, const std::string& nm, int8_t st, Point<int16_t> p)
			: cid(c), look(lk), level(l), job(j), name(nm), stance(st), position(p) {}

		int32_t getcid() const
		{
			return cid;
		}

		Type type() const override
		{
			return CHARACTER;
		}

		OtherChar* instantiate() const
		{
			return new OtherChar(cid, look, level, job, name, stance, position);
		}

	private:
		int32_t cid;
		uint8_t level;
		int16_t job; 
		std::string name;
		int8_t stance;
		Point<int16_t> position;
		LookEntry look;
	};
}