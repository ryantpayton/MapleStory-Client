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
#include "Cursor.h"

#include "nlnx\nx.hpp"

namespace jrc
{
	Cursor::Cursor()
	{
		state = IDLE;
	}

	void Cursor::init()
	{
		nl::node cursor = nl::nx::ui["Basic.img"]["Cursor"];
		for (auto& key : animations.getkeys())
		{
			animations[key] = cursor[std::to_string(key)];
		}
	}

	void Cursor::draw(float alpha) const
	{
		animations[state].draw(position, alpha);
	}

	void Cursor::update()
	{
		animations[state].update();
	}

	void Cursor::setstate(State s)
	{
		if (state != s)
		{
			state = s;
			animations[state].reset();
		}
	}

	void Cursor::setposition(Point<int16_t> pos)
	{
		position = pos;
	}

	Cursor::State Cursor::getstate() const
	{
		return state;
	}

	Point<int16_t> Cursor::getposition() const
	{
		return position;
	}
}