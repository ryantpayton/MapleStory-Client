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
#include "OtherChar.h"

#include "../Constants.h"

namespace ms
{
	OtherChar::OtherChar(std::int32_t id, const CharLook& lk, std::uint8_t lvl, std::int16_t jb, const std::string& nm, std::int8_t st, Point<int16_t> pos) : Char(id, lk, nm)
	{
		level = lvl;
		job = jb;
		set_position(pos);

		lastmove.xpos = pos.x();
		lastmove.ypos = pos.y();
		lastmove.newstate = st;
		timer = 0;

		attackspeed = 6;
		attacking = false;
	}

	std::int8_t OtherChar::update(const Physics& physics)
	{
		if (timer > 1)
		{
			timer--;
		}
		else if (timer == 1)
		{
			if (!movements.empty())
			{
				lastmove = movements.front();
				movements.pop();
			}
			else
			{
				timer = 0;
			}
		}

		if (!attacking)
		{
			std::uint8_t laststate = lastmove.newstate;
			set_state(laststate);
		}

		phobj.hspeed = lastmove.xpos - phobj.crnt_x();
		phobj.vspeed = lastmove.ypos - phobj.crnt_y();
		phobj.move();

		physics.get_fht().update_fh(phobj);

		bool aniend = Char::update(physics, get_stancespeed());

		if (aniend && attacking)
			attacking = false;

		return get_layer();
	}

	void OtherChar::send_movement(const std::vector<Movement>& newmoves)
	{
		movements.push(newmoves.back());

		if (timer == 0)
		{
			constexpr std::uint16_t DELAY = 50;
			timer = DELAY;
		}
	}

	void OtherChar::update_skill(std::int32_t skillid, std::uint8_t skilllevel)
	{
		skilllevels[skillid] = skilllevel;
	}

	void OtherChar::update_speed(std::uint8_t as)
	{
		attackspeed = as;
	}

	void OtherChar::update_look(const LookEntry& newlook)
	{
		look = newlook;

		std::uint8_t laststate = lastmove.newstate;
		set_state(laststate);
	}

	std::int8_t OtherChar::get_integer_attackspeed() const
	{
		return attackspeed;
	}

	std::uint16_t OtherChar::get_level() const
	{
		return level;
	}

	std::int32_t OtherChar::get_skilllevel(std::int32_t skillid) const
	{
		auto iter = skilllevels.find(skillid);

		if (iter == skilllevels.end())
			return 0;

		return iter->second;
	}
}