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
#include "Cursor.h"
#include "nlnx\nx.hpp"

namespace IO
{
	Animation& getanimation(Cursor::Mousestate state)
	{
		static std::map<Cursor::Mousestate, Animation> animations;
		if (!animations.count(state))
		{
			animations[state] = Animation(nl::nx::ui["Basic.img"]["Cursor"][std::to_string(state)]);
		}
		return animations[state];
	}

	Cursor::Cursor()
	{
		state = MST_IDLE;
	}

	Cursor::~Cursor() {}

	void Cursor::draw(float inter) const
	{
		using Graphics::DrawArgument;
		getanimation(state).draw(DrawArgument(position), inter);
	}

	void Cursor::update()
	{
		getanimation(state).update();
	}

	void Cursor::setstate(Mousestate s)
	{
		state = s;
	}

	void Cursor::setposition(vector2d<int16_t> pos)
	{
		position = pos;
	}

	Cursor::Mousestate Cursor::getstate() const
	{
		return state;
	}

	vector2d<int16_t> Cursor::getposition() const
	{
		return position;
	}
}