//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "MapObjects.h"

#include "../Combat/Attack.h"
#include "../Combat/SpecialMove.h"
#include "../Spawn.h"

#include <queue>

namespace jrc
{
	// A collection of mobs on a map.
	class MapMobs
	{
	public:
		// Draw all mobs on a layer.
		void draw(Layer::Id layer, double viewx, double viewy, float alpha) const;
		// Update all mobs.
		void update(const Physics& physics);

		// Spawn a new mob.
		void spawn(MobSpawn&& spawn);
		// Kill a mob.
		void remove(int32_t oid, int8_t effect);
		// Remove all mobs.
		void clear();

		// Update who a mob is controlled by.
		void set_control(int32_t oid, bool control);
		// Update a mob's hp display.
		void send_mobhp(int32_t oid, int8_t percent, uint16_t playerlevel);
		// Update a mob's movements.
		void send_movement(int32_t oid, Point<int16_t> start, std::vector<Movement>&& movements);

		// Calculate the results of an attack.
		AttackResult send_attack(const Attack& attack);
		// Applies damage to a mob.
		void apply_damage(int32_t oid, int32_t damage, bool toleft,
			const AttackUser& user, const SpecialMove& move);

		// Check if the mob with the specified oid exists.
		bool contains(int32_t oid) const;
		// Return the id of the first mob who collides with the object.
		int32_t find_colliding(const MovingObject& moveobj) const;
		// Create an attack by the specified mob.
		MobAttack create_attack(int32_t oid) const;
		// Return the position of a mob.
		Point<int16_t> get_mob_position(int32_t oid) const;
		// Return the head position of a mob.
		Point<int16_t> get_mob_head_position(int32_t oid) const;

	private:
		std::vector<int32_t> find_closest(Rectangle<int16_t> range, Point<int16_t> origin, uint8_t mobcount) const;

		MapObjects mobs;

		std::queue<MobSpawn> spawns;
	};
}

