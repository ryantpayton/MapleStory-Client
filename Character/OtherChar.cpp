//////////////////////////////////////////////////////////////////////////////
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
#include "OtherChar.h"
#include "Constants.h"

namespace Character
{
	OtherChar::OtherChar(int32_t id, CharLook lk, uint8_t lvl,
		int16_t jb, string nm, int8_t st, Point<int16_t> pos) : Char(id, lk, nm) {

		level = lvl;
		job = jb;
		setposition(pos);

		lastmove.xpos = pos.x();
		lastmove.ypos = pos.y();
		lastmove.newstate = st;
	}

	int8_t OtherChar::update(const Physics& physics)
	{
		if (movements.size() > 0)
		{
			lastmove = movements.front();
			movements.erase(movements.begin());
		}

		uint8_t laststate = lastmove.newstate;
		if (laststate % 2 == 1)
		{
			setflip(false);
			laststate -= 1;
		}
		else
		{
			setflip(true);
		}
		setstate(byvalue(laststate));

		phobj.x = lastmove.xpos;
		phobj.y = lastmove.ypos;

		physics.getfht().updatefh(phobj);

		return Char::update(physics);
	}

	void OtherChar::sendmovement(const vector<Movement>& mvts)
	{
		for (auto& mv : mvts)
		{
			movements.push_back(mv);
		}
	}
}
