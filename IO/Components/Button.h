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
#include "Util\rectangle2d.h"
#include <cstdint>

namespace IO
{
	using std::int16_t;
	using Util::vector2d;
	using Util::rectangle2d;

	// A button which can be used in an UIElement. Base class for different button types.
	class Button
	{
	public:
		enum State
		{
			NORMAL,
			DISABLED,
			MOUSEOVER,
			PRESSED
		};

		virtual ~Button() {}
		virtual void draw(vector2d<int16_t>) const = 0;
		virtual rectangle2d<int16_t> bounds(vector2d<int16_t>) const = 0;
		void setposition(vector2d<int16_t> p) { position = p; }
		void setstate(State s) { state = s; }
		void setactive(bool a) { active = a; }
		bool isactive() const { return active && state != DISABLED; }
		State getstate() const { return state; }
	protected:
		State state;
		vector2d<int16_t> position;
		bool active;
	};
}

