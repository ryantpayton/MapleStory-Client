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
#include "Char.h"
#include "Gameplay\Playable.h"
#include "Gameplay\Maplemap\MapInfo.h"

namespace Character
{
	using IO::Keyaction;
	using Gameplay::Playable;
	using Gameplay::Seat;
	using Gameplay::Ladder;

	// Interface for a class that is both playable and a character. 
	// Currently only used by the player, but could be extended to control androids or player npcs.
	class PlayableChar : public Playable, public Char
	{
	public:
		// Change players position to the seat's position and stance to Char::SIT.
		virtual void setseat(const Seat* seat) = 0;
		// Change players xpos to the ladder x and change stance to Char::LADDER or Char::ROPE.
		virtual void setladder(const Ladder* ladder) = 0;

		// Returns the walking force.
		virtual float getwforce() const = 0;
		// Returns the jumping force.
		virtual float getjforce() const = 0;
		// Returns the climbing force.
		virtual float getclimbforce() const = 0;
		// Returns the flying force.
		virtual float getflyforce() const = 0;

		// Returns if the keyaction is active.
		virtual bool keydown(Keyaction keycode) const = 0;
		// Returns the ladder.
		virtual const Ladder* getladder() const = 0;
	};
}