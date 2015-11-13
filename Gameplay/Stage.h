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

	// Represents the game below the ui. Contains everything map-related.
	class Stage : public StageInterface
	{
	public:
		Stage(ClientInterface&);

		// Calls 'init()' of members to preload assets.
		void init();
		// Call 'draw()' of  all objects on stage.
		void draw(float inter) const;
		// Calls 'update()' of all objects on stage.
		void update();

		// Adds a player object with the given properties.
		bool loadplayer(int32_t charid) override;
		// Loads the map to display. 
		void loadmap(int32_t mapid) override;
		// Repositions the player and reactivates the stage after loading.
		// Also plays the bgm associated with the newly loaded map.
		void respawn() override;
		void sendkey(IO::Keytype keytype, int32_t keycode, bool pressed) override;

		// Returns a reference to the npcs on the current map.
		MapNpcs& getnpcs() override;
		// Returns a reference to the other characters on the current map.
		MapChars& getchars() override;
		// Returns a reference to the mobs on the current map.
		MapMobs& getmobs() override;
		// Returns a reference to the Player.
		Player& getplayer() override;

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

		Camera camera;
		Physics physics;

		Player player;
		Playable* playable;
		bool active;
		int32_t currentmapid;
	};
}

