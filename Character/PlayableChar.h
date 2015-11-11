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
#include "Gameplay\Playable.h"
#include "Gameplay\Maplemap\Char.h"
#include "Gameplay\Physics\PhysicsObject.h"

namespace Character
{
	using::IO::Keyaction;
	using::Gameplay::Playable;
	using::Gameplay::PhysicsObject;

	// Interface for a class that is both playable and a character. 
	// Currently only used by the player, but could be extended to control androids or player npcs.
	class PlayableChar : public Playable, public Char
	{
	public:
		// Returns the walking force.
		virtual float getwforce() const = 0;
		// Returns the jumping force.
		virtual float getjforce() const = 0;
		// Returns the flying force.
		virtual float getflyforce() const = 0;
		// Returns if the keyaction is active.
		// Parameters: Keyaction(maple-keycode)
		virtual bool keydown(Keyaction) const = 0;
		// Returns a reference to a PhysicsObject, used to apply forces or check physics properties.
		virtual PhysicsObject& getphobj() = 0;
	};
}