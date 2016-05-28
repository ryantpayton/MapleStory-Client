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

#include "..\Constants.h"

namespace jrc
{
	OtherChar::OtherChar(int32_t id, const CharLook& lk, uint8_t lvl,
		int16_t jb, const std::string& nm, int8_t st, Point<int16_t> pos) : Char(id, lk, nm) {

		level = lvl;
		job = jb;
		setposition(pos);

		lastmove.xpos = pos.x();
		lastmove.ypos = pos.y();
		lastmove.newstate = st;

		attacking = false;
	}

	int8_t OtherChar::update(const Physics& physics)
	{
		if (movements.size() > 0)
		{
			lastmove = movements.front();
			movements.pop();
		}

		if (!attacking)
		{
			uint8_t laststate = lastmove.newstate;
			setstate(laststate);
		}

		phobj.hspeed = lastmove.xpos - phobj.x.get();
		phobj.vspeed = lastmove.ypos - phobj.y.get();

		phobj.move();

		physics.getfht().updatefh(phobj);

		bool aniend = Char::update(physics, getstancespeed());
		if (aniend && attacking)
		{
			attacking = false;
		}

		return getlayer();
	}

	void OtherChar::sendmovement(std::queue<Movement> newmoves)
	{
		movements = newmoves;
	}

	void OtherChar::updateskill(int32_t skillid, uint8_t skilllevel)
	{
		skilllevels[skillid] = skilllevel;
	}

	void OtherChar::updateattack(uint8_t as)
	{
		attackspeed = as;
	}

	void OtherChar::updatelook(const LookEntry& newlook)
	{
		look = newlook;

		uint8_t laststate = lastmove.newstate;
		setstate(laststate);
	}

	uint16_t OtherChar::getlevel() const
	{
		return level;
	}

	int32_t OtherChar::getskilllevel(int32_t skillid) const
	{
		return skilllevels.count(skillid) ? skilllevels.at(skillid) : 0;
	}

	float OtherChar::getattackspeed() const
	{
		float delay = static_cast<float>(attackspeed);
		return 1.7f - (delay / 10);
	}
}
