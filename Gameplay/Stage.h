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

namespace jrc
{
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
		void queuespawn(const Spawn* spawn);
		// Show an attack.
		void showattack(int32_t cid, const AttackResult& attack);
		// Show a buff effect.
		void showbuff(int32_t cid, int32_t skillid, int8_t level);
		// Show a buff effect.
		void showplayerbuff(int32_t skillid);

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
		void usemove(int32_t moveid);
		void applymove(const SpecialMove& move);
		void applyusemovement(const SpecialMove& move);
		void applyresultmovement(const SpecialMove& move, const AttackResult& result);
		void applyrush(const AttackResult& result);
		const SpecialMove& getmove(int32_t moveid);

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

		std::unordered_map<int32_t, std::unique_ptr<SpecialMove>> specialmoves;
	};
}

