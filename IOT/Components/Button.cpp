//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "Button.h"

namespace ms
{
	bool Button::in_combobox(Point<int16_t>)
	{
		return false;
	}

	uint16_t Button::get_selected() const
	{
		return uint16_t();
	}

	void Button::set_position(Point<int16_t> pos)
	{
		position = pos;
	}

	void Button::set_state(State s)
	{
		if (s == Button::State::IDENTITY)
			return;

		state = s;
	}

	void Button::set_active(bool a)
	{
		active = a;
	}

	void Button::toggle_pressed()
	{
		pressed = !pressed;
	}

	bool Button::is_active() const
	{
		return active && state != Button::State::DISABLED;
	}

	Button::State Button::get_state() const
	{
		return state;
	}

	bool Button::is_pressed() const
	{
		return pressed;
	}
}