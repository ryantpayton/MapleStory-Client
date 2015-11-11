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
#include "IO\KeyTarget.h"
#include "Character\Look\CharLook.h"
#include "Character\Charstats.h"
#include "Audio\Audioplayer.h"
#include "Gameplay\Maplemap\Mapmobs.h"
#include "Character\Player.h"

namespace Gameplay
{
	using::Character::CharLook;
	using::Character::Charstats;
	using::Character::Player;
	using::Audio::Audioplayer;
	using::IO::KeyTarget;
	using::Gameplay::Maplemap::Mapmobs;

	// Interface for the 'Stage' class which represents the current map.
	// Inherits from 'KeyTarget' to process key inputs.
	class StageInterface : public KeyTarget
	{
	public:
		virtual ~StageInterface() {}
		// Adds a player object with the given properties.
		// Parameters: int(objectid), CharLook(look to use), Charstats(stats to use)
		virtual void loadplayer(int32_t, CharLook, Charstats) = 0;
		// Loads the map to display. 
		// Parameters: int(mapid)
		virtual void loadmap(int32_t) = 0;
		// Repositions the player and reactivates the stage after loading.
		// Also plays the bgm associated with the newly loaded map.
		// Parameters: Audioplayer&(musicplayer to play the bgm with)
		virtual void respawn(Audioplayer&) = 0;
		// Spawn an npc on the current map.
		// Parameters: int32_t(id), int32_t(oid), bool(is flipped), 
		// uint16_t(starting foothoold), int32_t(x), int32_t(y)
		virtual void addnpc(int32_t, int32_t, bool, uint16_t, int32_t, int32_t) = 0;
		// Returns a reference to the mobs on the current map.
		virtual Mapmobs& getmobs() = 0;
		// Returns a reference to the Player.
		virtual Player& getplayer() = 0;
	};
}