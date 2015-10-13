/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "Animation.h"
#include "ptrmap.h"

using namespace Graphics;

namespace IO
{
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

	class Cursor
	{
	public:
		Cursor();
		~Cursor() {}
		void init();
		void draw();
		void update(short);
		Mousestate getstate() { return state; }
		void setstate(Mousestate s) { state = s; }
		void setposition(vector2d<int> p) { position = p; }
	private:
		ptrmap<Mousestate, Animation*> animations;
		Mousestate state;
		vector2d<int> position;
	};
}

