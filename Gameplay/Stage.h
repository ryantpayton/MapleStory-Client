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
#include "Camera.h"
#include "Spawn.h"
#include "Physics\Physics.h"
#include "Maplemap\MapInfo.h"
#include "Maplemap\MapLayers.h"
#include "Maplemap\MapBackgrounds.h"
#include "Maplemap\MapPortals.h"
#include "Maplemap\MapChars.h"
#include "Maplemap\MapMobs.h"
#include "Maplemap\MapNpcs.h"
#include "Maplemap\MapDrops.h"
#include "Character\Player.h"
#include "Util\Singleton.h"
#include <queue>

namespace Gameplay
{
	using std::queue;
	using Character::Player;
	using Character::Char;

	class Stage : public Singleton<Stage>
	{
	public:
		Stage();

		// Adds a player object with the given properties.
		bool loadplayer(int32_t charid);
		// Set the map which will be loaded with the next call to reload().
		void setmap(uint8_t portalid, int32_t mapid);
		// Loads the map to display. 
		void reload();
		// Parses and displays a map from a packet.
		void parsemap(InPacket& recv);
		// Remove all map objects and graphics.
		void clear();

		// Call 'draw()' of all objects on stage.
		void draw(float inter) const;
		// Calls 'update()' of all objects on stage.
		void update();

		// Add a spawn to the spawn queue.
		void queuespawn(const Spawn* spawn);

		// Send key input to the stage.
		void sendkey(Keyboard::Keytype keytype, int32_t keycode, bool pressed);
		// Send mouse input to the stage.
		Cursor::State sendmouse(bool pressed, Point<int16_t> position);

		// Check if the specified id is the player's id.
		bool isplayer(int32_t cid) const;

		// Returns a reference to the npcs on the current map.
		MapNpcs& getnpcs();
		// Returns a reference to the other characters on the current map.
		MapChars& getchars();
		// Returns a reference to the mobs on the current map.
		MapMobs& getmobs();
		// Returns a reference to the drops on the current map.
		MapDrops& getdrops();
		// Returns a reference to the Player.
		Player& getplayer();

		// Return a pointer to a character, possibly the player.
		Optional<Char> getcharacter(int32_t cid);

	private:
		void pollspawns();
		void loadmap();
		void respawn();
		void checkportals();
		void checkseats();
		void checkladders(bool up);
		void checkdrops();
		void useattack();
		void useskill(int32_t skillid);

		enum State
		{
			INACTIVE,
			TRANSITION,
			ACTIVE
		};

		Camera camera;
		Physics physics;
		Player player;

		MapInfo mapinfo;
		MapLayers layers;
		MapBackgrounds backgrounds;
		MapPortals portals;
		MapNpcs npcs;
		MapChars chars;
		MapMobs mobs;
		MapDrops drops;

		queue<unique_ptr<const Spawn>> spawnqueue;

		State state;
		Optional<Playable> playable;
		int32_t mapid;
		uint8_t portalid;

		unordered_map<int32_t, Skill> skillcache;
	};
}

