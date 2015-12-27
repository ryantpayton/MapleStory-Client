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
#include "Graphics\Animation.h"

namespace IO
{
	using std::uint16_t;
	using std::int16_t;
	using std::map;
	using Util::vector2d;
	using Graphics::Animation;

	// Class that represents the mouse cursor.
	class Cursor
	{
	public:
		// Maple-cursorstates, linked to the cursor's animation.
		enum Mousestate
		{
			MST_IDLE = 0,
			MST_CANCLICK = 1,
			MST_GAME = 2,
			MST_HOUSE = 3,
			MST_CANCLICK2 = 4,
			MST_CANGRAB = 5,
			MST_GIFT = 6,
			MST_VSCROLL = 7,
			MST_HSCROLL = 8,
			MST_VSCROLLIDLE = 9,
			MST_HSCROLLIDLE = 10,
			MST_GRABBING = 11,
			MST_CLICKING = 12,
			MST_RCLICK = 13
		};

		Cursor();
		~Cursor();

		void draw(float) const;
		void update();
		void setstate(Mousestate);
		void setposition(vector2d<int16_t>);
		Mousestate getstate() const;
		vector2d<int16_t> getposition() const;

	private:
		Mousestate state;
		vector2d<int16_t> position;
	};
}

