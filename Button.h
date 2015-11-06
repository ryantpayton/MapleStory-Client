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
#include "rectangle2d.h"
#include <cstdint>

namespace IO
{
	// Buttonstates that are used in game data.
	enum Buttonstate
	{
		BTS_NORMAL,
		BTS_DISABLED,
		BTS_MOUSEOVER,
		BTS_PRESSED
	};

	using::std::int32_t;
	using::Util::vector2d;
	using::Util::rectangle2d;

	// A button which can be used in an UIElement. Base class for different button types.
	class Button
	{
	public:
		virtual ~Button() {}
		virtual void draw(vector2d<int32_t>) const = 0;
		virtual rectangle2d<int32_t> bounds(vector2d<int32_t>) const = 0;
		void setposition(vector2d<int32_t> p) { position = p; }
		void setstate(Buttonstate s) { state = s; }
		void setactive(bool a) { active = a; }
		bool isactive() const { return active && state != BTS_DISABLED; }
		Buttonstate getstate() const { return state; }
	protected:
		Buttonstate state;
		vector2d<int32_t> position;
		bool active;
	};
}

