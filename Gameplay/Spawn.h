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
#include "Physics\Physics.h"
#include <cstdint>

namespace Gameplay
{
	class Spawn
	{
	public:
		enum Type
		{
			NPC,
			MOB,
			REACTOR,
			CHARACTER
		};

		virtual ~Spawn() {}

		virtual Type type() const = 0;
		virtual MapObject* instantiate(const Physics& physics) const = 0;
	};

	class NpcSpawn : public Spawn
	{
	public:
		NpcSpawn(int32_t o, int32_t i, Point<int16_t> p, bool fl, uint16_t f)
		{
			oid = o;
			id = i;
			position = p;
			flip = fl;
			fh = f;
		}

		Type type() const override
		{
			return NPC;
		}

		MapObject* instantiate(const Physics& physics) const override
		{
			Point<int16_t> spawnposition = physics.getgroundbelow(position);
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
		MobSpawn(int32_t o, int32_t i, bool c, int8_t st, uint16_t f,
			bool ns, int8_t t, Point<int16_t> p) {

			oid = o;
			id = i;
			control = c;
			stance = st;
			fh = f;
			newspawn = ns;
			team = t;
			position = p;
		}

		Type type() const override
		{
			return MOB;
		}

		MapObject* instantiate(const Physics&) const override
		{
			return new Mob(oid, id, control, stance, fh, newspawn, team, position);
		}

	private:
		int32_t oid;
		int32_t id;
		bool control;
		int8_t stance;
		uint16_t fh;
		bool newspawn;
		int8_t team;
		Point<int16_t> position;
	};
}