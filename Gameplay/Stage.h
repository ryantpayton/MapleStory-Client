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
#include "Maplemap\MapChars.h"
#include "Maplemap\MapMobs.h"
#include "Maplemap\MapNpcs.h"
#include "Character\Player.h"

namespace Gameplay
{
	namespace Stage
	{
		using Character::Player;
		using Character::Char;

		// Call 'draw()' of  all objects on stage.
		void draw(float inter);
		// Calls 'update()' of all objects on stage.
		void update();

		// Adds a player object with the given properties.
		bool loadplayer(int32_t charid);
		// Loads the map to display. 
		void loadmap(int32_t mapid);
		// Repositions the player and reactivates the stage after loading.
		// Also plays the bgm associated with the newly loaded map.
		void respawn();
		// Send a key input to the stage.
		void sendkey(IO::Keyboard::Keytype keytype, int32_t keycode, bool pressed);

		// Returns a reference to the npcs on the current map.
		MapNpcs& getnpcs();
		// Returns a reference to the other characters on the current map.
		MapChars& getchars();
		// Returns a reference to the mobs on the current map.
		MapMobs& getmobs();
		// Returns a reference to the Player.
		Player& getplayer();

		// Return a pointer to a character, possibly the player.
		Char* getcharacter(int32_t cid);
	}
}

