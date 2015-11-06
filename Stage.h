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
#include "KeyTarget.h"
#include "ClientInterface.h"
#include "Mapinfo.h"
#include "Maplayers.h"
#include "Mapportals.h"
#include "Mapchars.h"
#include "Mapnpcs.h"
#include "Camera.h"
#include "Physics.h"
#include "Player.h"
#include "Audioplayer.h"

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
		// Parameters: int(objectid), CharLook(look to use), Charstats(stats to use)
		void loadplayer(int32_t, CharLook, Charstats);
		// Loads the map to display. 
		// Parameters: int(mapid)
		void loadmap(int32_t);
		// Repositions the player and reactivates the stage after loading.
		// Also plays the bgm associated with the newly loaded map.
		// Parameters: Audioplayer&(musicplayer to play the bgm with)
		void respawn(Audioplayer&);
		// Call 'draw()' of  all objects on stage.
		void draw() const;
		// Calls 'update()' of all objects on stage.
		// Parameters: short(delay-per-frame)
		void update(uint16_t);
		// Overrides 'sendkey()' of 'KeyTarget' to process key inputs.
		// Will delegate inputs of Keytype 'KT_ACTION' to a targeted 'Playable'.
		// Parameters: Keytype(type of key), int(maple-keycode), bool(is key down)
		void sendkey(Keytype, int32_t, bool);
		// Returns a reference to the npcs on the current map.
		Mapnpcs& getnpcs();
		// Returns a reference to the Player.
		Player& getplayer();
	private:
		void checkportal();
		void checkseats();

		ClientInterface& client;
		Mapinfo mapinfo;
		Maplayers layers;
		Mapportals portals;
		Mapchars chars;
		Mapnpcs npcs;
		Camera camera;
		Physics physics;
		Player player;
		Playable* playable;
		bool active;
		int32_t mapid;
		int32_t playerid;
	};
}

