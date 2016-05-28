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
#include "..\..\Util\Rectangle.h"

#include <cstdint>

namespace jrc
{
	// Base class for different button types.
	class Button
	{
	public:
		static const size_t NUM_STATES = 4;
		enum State
		{
			NORMAL,
			DISABLED,
			MOUSEOVER,
			PRESSED
		};

		virtual ~Button() {}

		virtual void draw(Point<int16_t> parentpos) const = 0;
		virtual Rectangle<int16_t> bounds(Point<int16_t> parentpos) const = 0;

		void setposition(Point<int16_t> position);
		void setstate(State state);
		void setactive(bool active);

		bool isactive() const;
		State getstate() const;

	protected:
		State state;
		Point<int16_t> position;
		bool active;
	};
}

