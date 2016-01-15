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
#include "Program\Constants.h"

namespace Character
{
	OtherChar::OtherChar(int32_t id, const LookEntry& lk, uint8_t lvl,
		int16_t jb, string nm, int8_t st, vector2d<int16_t> pos) {

		oid = id;
		look = CharLook(lk);
		level = lvl;
		job = jb;
		name = nm;
		setposition(pos.x(), pos.y());

		lastmove.xpos = pos.x();
		lastmove.ypos = pos.y();
		lastmove.newstate = st;

		namelabel = Text(Text::A13M, Text::CENTER, Text::WHITE);
		namelabel.settext(name);
		namelabel.setback(Text::NAMETAG);
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
		setstance(static_cast<Stance>(laststate));

		phobj.lastx = phobj.fx;
		phobj.lasty = phobj.fy;
		phobj.fx = static_cast<float>(lastmove.xpos);
		phobj.fy = static_cast<float>(lastmove.ypos);

		phobj.hspeed = phobj.fx - phobj.lastx;
		phobj.vspeed = phobj.fy - phobj.lasty;

		physics.getfht().updatefh(phobj);
		look.update(Constants::TIMESTEP);

		return phobj.fhlayer;
	}

	void OtherChar::sendmovement(const vector<MovementFragment>& mvts)
	{
		for (auto& mv : mvts)
		{
			movements.push_back(mv);
		}
	}
}
