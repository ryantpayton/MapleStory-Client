//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "Button.h"

namespace jrc
{
	// An invisible button which is only defined by it's area.
	class AreaButton : public Button
	{
	public:
		AreaButton(Point<int16_t> position, Point<int16_t> dimensions);

		void draw(Point<int16_t>) const {}
		Rectangle<int16_t> bounds(Point<int16_t> parentpos) const;
		int16_t width() const;
		Point<int16_t> origin() const;
		Cursor::State send_cursor(bool, Point<int16_t>) { return Cursor::State::IDLE; };
		bool remove_cursor(bool, Point<int16_t>) { return false; };
		bool in_combobox(Point<int16_t>) { return false; };
		uint16_t get_selected() const { return 0; };

	private:
		Point<int16_t> dimension;
	};
}