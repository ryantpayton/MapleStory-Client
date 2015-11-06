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
#include "Keyboard.h"

namespace IO
{
	// Transition to use for fading out and back in.
	enum Transition
	{
		TRS_LOGIN,
		TRS_MAP
	};

	// Interface for classes that display the game to the user.
	class Window
	{
	public:
		virtual ~Window() {}
		// Transition between two game screens, using the transition type specified.
		// Parameters: Transition(effect to use)
		virtual void fadeout(Transition) = 0;
		// Return a reference to the class storing key mappings.
		virtual Keyboard& getkeyboard() = 0;
	};
}