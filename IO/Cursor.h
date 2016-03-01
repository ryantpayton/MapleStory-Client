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
#include "Util\Enum.h"
#include "Graphics\Animation.h"

namespace IO
{
	using std::map;
	using Graphics::Animation;

	// Class that represents the mouse cursor.
	class Cursor
	{
	public:
		// Maple-cursorstates, linked to the cursor's animation.
		enum State
		{
			IDLE = 0,
			CANCLICK = 1,
			GAME = 2,
			HOUSE = 3,
			CANCLICK2 = 4,
			CANGRAB = 5,
			GIFT = 6,
			VSCROLL = 7,
			HSCROLL = 8,
			VSCROLLIDLE = 9,
			HSCROLLIDLE = 10,
			GRABBING = 11,
			CLICKING = 12,
			RCLICK = 13
		};

		static EnumIterator<State> getstateit()
		{
			return EnumIterator<State>(IDLE, RCLICK);
		}

		Cursor();

		void init();

		void draw(float inter) const;
		void update();
		void setstate(State state);
		void setposition(Point<int16_t> position);
		State getstate() const;
		Point<int16_t> getposition() const;

	private:
		map<State, Animation> animations;

		State state;
		Point<int16_t> position;
	};
}

