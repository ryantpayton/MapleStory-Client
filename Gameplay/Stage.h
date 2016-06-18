/////////////////////////////////////////////////////////////////////////////
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
#include "Camera.h"
#include "Spawn.h"

#include "Maplemap\MapInfo.h"
#include "Maplemap\MapLayers.h"
#include "Maplemap\MapBackgrounds.h"
#include "Maplemap\MapPortals.h"
#include "Maplemap\MapChars.h"
#include "Maplemap\MapMobs.h"
#include "Maplemap\MapNpcs.h"
#include "Maplemap\MapDrops.h"
#include "Physics\Physics.h"

#include "..\Character\Player.h"
#include "..\Util\Singleton.h"

#include <queue>
#include <list>

namespace jrc
{
	class DelayedAttack
	{
	public:
		DelayedAttack(const AttackResult& a, int32_t c)
			: attack(a), char_id(c) {
		
			delay = 50;
		}

		bool update()
		{
			delay--;
			return delay <= 1;
		}

		AttackResult attack;
		int32_t char_id;

	private:
		int16_t delay;
	};


	class Stage : public Singleton<Stage>
	{
	public:
		Stage();

		// Contruct the player from a character entry.
		void loadplayer(const CharEntry& entry);
		// Set the map which will be loaded with the next call to reload().
		void setmap(uint8_t portalid, int32_t mapid);
		// Loads the map to display. 
		void reload();
		// Remove all map objects and graphics.
		void clear();

		// Call 'draw()' of all objects on stage.
		void draw(float alpha) const;
		// Calls 'update()' of all objects on stage.
		void update();

		// Add a spawn to the spawn queue.
		void push_spawn(std::unique_ptr<const Spawn> spawn);
		// Show an attack.
		void show_attack(int32_t cid, const AttackResult& attack);
		// Show a buff effect.
		void show_buff(int32_t cid, int32_t skillid, int8_t level);
		// Show a buff effect.
		void show_player_buff(int32_t skillid);

		// Make the player use a special move.
		void use_move(int32_t moveid);

		// Send key input to the stage.
		void send_key(Keyboard::Keytype keytype, int32_t keycode, bool pressed);
		// Send mouse input to the stage.
		Cursor::State send_cursor(bool pressed, Point<int16_t> position);

		// Check if the specified id is the player's id.
		bool is_player(int32_t cid) const;

		// Returns a reference to the npcs on the current map.
		MapNpcs& get_npcs();
		// Returns a reference to the other characters on the current map.
		MapChars& get_chars();
		// Returns a reference to the mobs on the current map.
		MapMobs& get_mobs();
		// Returns a reference to the drops on the current map.
		MapDrops& get_drops();
		// Returns a reference to the Player.
		Player& get_player();

		// Return a pointer to a character, possibly the player.
		Optional<Char> get_character(int32_t cid);

	private:
		void pollspawns();
		void pollattacks();
		void loadmap();
		void respawn();
		void check_portals();
		void check_seats();
		void check_ladders(bool up);
		void check_drops();
		void apply_move(const SpecialMove& move);
		void apply_use_movement(const SpecialMove& move);
		void apply_result_movement(const SpecialMove& move, const AttackResult& result);
		void apply_rush(const AttackResult& result);
		const SpecialMove& get_move(int32_t moveid);

		enum State
		{
			INACTIVE,
			TRANSITION,
			ACTIVE
		};

		Camera camera;
		Physics physics;
		Player player;

		Optional<Playable> playable;
		State state;
		int32_t mapid;
		uint8_t portalid;

		MapInfo mapinfo;
		MapLayers layers;
		MapBackgrounds backgrounds;
		MapPortals portals;
		MapNpcs npcs;
		MapChars chars;
		MapMobs mobs;
		MapDrops drops;

		std::queue<std::unique_ptr<const Spawn>> spawnqueue;
		std::list<DelayedAttack> attackqueue;
		std::unordered_map<int32_t, std::unique_ptr<SpecialMove>> specialmoves;
	};
}

