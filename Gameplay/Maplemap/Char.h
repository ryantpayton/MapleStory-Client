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
#include "Gameplay\Maplemap\Mapobject.h"
#include "Character\Look\CharLook.h"

namespace Character
{
	using::Gameplay::Mapobject;

	// Player stances which determine animation and state. Values are used in movement packets (add 1 if facing left).
	enum CharStance
	{
		PST_WALK = 2,
		PST_STAND = 4,
		PST_FALL = 6,
		PST_ALERT = 8,
		PST_PRONE = 10,
		PST_SWIM = 12,
		PST_LADDER = 14,
		PST_ROPE = 16,
		PST_DIED = 18,
		PST_SIT = 20,
		PST_SKILL = 22
	};
	// Number of stances that are currently implemented.
	const size_t NUM_STANCES = 11;
	// Names of the character stances used in the game's files.
	const string stances[NUM_STANCES] =
	{
		"", "walk", "stand", "jump", "alert", "prone", "fly", "ladder", "rope",
		"dead", "sit"
	};
	// Interface for characters, e.g. the player and other clients on the same map.
	class Char : public Mapobject
	{
	public:
		virtual void setflip(bool) = 0;
		virtual void setstance(CharStance) = 0;
		virtual bool getflip() const = 0;
		virtual CharStance getstance() const = 0;
		virtual CharLook& getlook() = 0;
	};
}