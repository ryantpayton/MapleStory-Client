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
#include "Program\ClientInterface.h"
#include "Maplemap\MapInfo.h"
#include "Maplemap\MapLayer.h"
#include "Maplemap\MapPortals.h"
#include "Maplemap\Mapchars.h"
#include "Camera.h"
#include "Physics\Physics.h"
#include "Character\Player.h"

namespace Gameplay
{
	using::Journey::ClientInterface;
	using::IO::Keytype;

	// Represents the game below the ui. Contains everything map-related.
	class Stage : public StageInterface
	{
	public:
		// Initializes 'active' with false.
		Stage(ClientInterface&);
		// Calls 'init()' of members to preload assets.
		void init();
		// Adds a player object with the given properties.
		// Parameters: int(character id)
		bool loadplayer(int32_t);
		// Loads the map to display. 
		// Parameters: int(mapid)
		void loadmap(int32_t);
		// Repositions the player and reactivates the stage after loading.
		// Also plays the bgm associated with the newly loaded map.
		void respawn();
		// Call 'draw()' of  all objects on stage.
		void draw(float) const;
		// Calls 'update()' of all objects on stage.
		void update();
		// Overrides 'sendkey()' of 'KeyTarget' to process key inputs.
		// Will delegate inputs of Keytype 'KT_ACTION' to a targeted 'Playable'.
		// Parameters: Keytype(type of key), int(maple-keycode), bool(is key down)
		void sendkey(Keytype, int32_t, bool);
		// Returns a reference to the npcs on the current map.
		MapNpcs& getnpcs();
		// Returns a reference to the other characters on the current map.
		MapChars& getchars();
		// Returns a reference to the mobs on the current map.
		MapMobs& getmobs();
		// Returns a reference to the Player.
		Player& getplayer();
	private:
		void checkportals();
		void checkseats();
		void checkladders();

		ClientInterface& client;

		MapInfo mapinfo;
		map<uint8_t, MapLayer> layers;
		MapPortals portals;
		MapNpcs npcs;
		MapChars chars;
		MapMobs mobs;
		Player player;

		Camera camera;
		Physics physics;

		Playable* playable;
		bool active;
		int32_t currentmapid;
		int32_t playerid;
	};
}

