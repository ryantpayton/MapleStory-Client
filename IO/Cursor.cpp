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

namespace IO
{
	Cursor::Cursor()
	{
		state = IDLE;
	}

	void Cursor::init()
	{
		for (auto it = getstateit(); it.hasnext(); it.increment())
		{
			State st = it.get();
			animations[st] = Animation(nl::nx::ui["Basic.img"]["Cursor"][std::to_string(st)]);
		}
	}

	void Cursor::draw(float inter) const
	{
		if (!animations.count(state))
			return;

		using Graphics::DrawArgument;
		animations.at(state).draw(DrawArgument(position), inter);
	}

	void Cursor::update()
	{
		animations[state].update();
	}

	void Cursor::setstate(State s)
	{
		state = s;
	}

	void Cursor::setposition(vector2d<int16_t> pos)
	{
		position = pos;
	}

	Cursor::State Cursor::getstate() const
	{
		return state;
	}

	vector2d<int16_t> Cursor::getposition() const
	{
		return position;
	}
}