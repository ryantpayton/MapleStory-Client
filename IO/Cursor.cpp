/////////////////////////////////////////////////////////////////////////////
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
#include "Cursor.h"

#include "..\Constants.h"

#include <nlnx\nx.hpp>

namespace jrc
{
	Cursor::Cursor()
	{
		state = IDLE;
		hide_counter = 0;
	}

	void Cursor::init()
	{
		nl::node src = nl::nx::ui["Basic.img"]["Cursor"];
		for (auto iter : animations)
		{
			iter.second = src[iter.first];
		}
	}

	void Cursor::draw(float alpha) const
	{
		constexpr int32_t HIDE_AFTER = HIDE_TIME / Constants::TIMESTEP;

		if (hide_counter < HIDE_AFTER)
		{
			animations[state]
				.draw(position, alpha);
		}
	}

	void Cursor::update()
	{
		animations[state].update();

		switch (state)
		{
		case CANCLICK:
		case CANCLICK2:
		case CANGRAB:
		case CLICKING:
		case GRABBING:
			hide_counter = 0;
			break;
		default:
			hide_counter++;
			break;
		}
	}

	void Cursor::set_state(State s)
	{
		if (state != s)
		{
			state = s;

			animations[state].reset();
			hide_counter = 0;
		}
	}

	void Cursor::set_position(Point<int16_t> pos)
	{
		position = pos;
		hide_counter = 0;
	}

	Cursor::State Cursor::get_state() const
	{
		return state;
	}

	Point<int16_t> Cursor::get_position() const
	{
		return position;
	}
}