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
#include "..\Graphics\Animation.h"
#include "..\Util\EnumMap.h"

namespace jrc
{
	// Class that represents the mouse cursor.
	class Cursor
	{
	public:
		// Maple-cursorstates, linked to the cursor's animation.
		enum State
		{
			IDLE,
			CANCLICK,
			GAME,
			HOUSE,
			CANCLICK2,
			CANGRAB,
			GIFT,
			VSCROLL,
			HSCROLL,
			VSCROLLIDLE,
			HSCROLLIDLE,
			GRABBING,
			CLICKING,
			RCLICK,
			LENGTH
		};

		Cursor();

		void init();

		void draw(float alpha) const;
		void update();
		void setstate(State state);
		void setposition(Point<int16_t> position);
		State getstate() const;
		Point<int16_t> getposition() const;

	private:
		EnumMap<State, Animation> animations;

		State state;
		Point<int16_t> position;
	};
}

