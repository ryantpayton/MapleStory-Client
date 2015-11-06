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
#include "nx.hpp"

namespace IO
{
	Cursor::Cursor()
	{
		position = vector2d<int32_t>();
		state = MST_IDLE;
	}

	Cursor::~Cursor() {}

	void Cursor::init()
	{
		using::nl::node;
		node cursornode = nl::nx::ui["Basic.img"]["Cursor"];
		for (Mousestate i = MST_IDLE; i <= MST_RCLICK; i = static_cast<Mousestate>(i + 1))
		{
			animations.add(i, new Animation(cursornode[std::to_string(i)]));
		}
	}

	void Cursor::draw() const
	{
		using::Graphics::PosArgument;
		const Animation* anim = animations.get(state);
		if (anim)
		{
			anim->draw(PosArgument(position));
		}
	}

	void Cursor::update(uint16_t dpf)
	{
		Animation* anim = animations.get(state);
		if (anim)
		{
			anim->update(dpf);
		}
	}

	void Cursor::setstate(Mousestate s)
	{
		state = s;
	}

	void Cursor::setposition(vector2d<int32_t> pos)
	{
		position = pos;
	}

	Mousestate Cursor::getstate() const
	{
		return state;
	}
}