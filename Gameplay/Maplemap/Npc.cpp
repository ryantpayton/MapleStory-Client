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
#include "Npc.h"
#include "nlnx\node.hpp"
#include "nlnx\nx.hpp"

namespace Gameplay
{
	Npc::Npc(int32_t id, int32_t o, bool fl, uint16_t f, bool cnt, Point<int16_t> position) 
		: MapObject(o) {

		string strid = std::to_string(id);
		strid.insert(0, 7 - strid.size(), '0');
		strid.append(".img");

		using nl::node;
		node src = nl::nx::npc[strid];
		node strsrc = nl::nx::string["Npc.img"][std::to_string(id)];

		string link = src["info"]["link"];
		if (link.size() > 0)
		{
			link.append(".img");
			src = nl::nx::npc[link];
		}

		node info = src["info"];

		hidename = info["hideName"].get_bool();
		mouseonly = info["talkMouseOnly"].get_bool();
		scripted = info["script"].size() > 0 || info["shop"].get_bool();

		for (node npcnode : src)
		{
			string state = npcnode.name();
			if (state != "info")
			{
				animations[state] = npcnode;
				states.push_back(state);
			}

			for (node speaknode : npcnode["speak"])
			{
				lines[state].push_back(strsrc[speaknode.get_string()]);
			}
		}

		name = strsrc["name"];
		func = strsrc["func"];

		namelabel = Text(Text::A13B, Text::CENTER, Text::YELLOW);
		namelabel.settext(name);
		namelabel.setback(Text::NAMETAG);
		funclabel = Text(Text::A13B, Text::CENTER, Text::YELLOW);
		funclabel.settext(func);
		funclabel.setback(Text::NAMETAG);

		npcid = id;
		flip = !fl;
		control = cnt;
		stance = "stand";

		phobj.fhid = f;
		setposition(position);
	}

	void Npc::draw(Point<int16_t> viewpos, float inter) const
	{
		Point<int16_t> absp = phobj.getposition(inter) + viewpos;
		if (animations.count(stance))
		{
			using Graphics::DrawArgument;
			animations.at(stance).draw(DrawArgument(absp, flip), inter);
		}
		if (!hidename)
		{
			namelabel.draw(absp);
			funclabel.draw(absp + Point<int16_t>(0, 18));
		}
	}

	int8_t Npc::update(const Physics& physics)
	{
		if (!active)
			return phobj.fhlayer;

		physics.moveobject(phobj);

		if (animations.count(stance))
		{
			bool aniend = animations.at(stance).update();
			if (aniend && states.size() > 0)
			{
				string newstate = states[random.nextint(states.size() - 1)];
				setstance(newstate);
			}
		}

		return phobj.fhlayer;
	}

	void Npc::setstance(string st)
	{
		if (stance != st)
		{
			stance = st;
			if (animations.count(stance))
			{
				animations.at(stance).reset();
			}
		}
	}

	bool Npc::isscripted() const
	{
		return scripted;
	}

	bool Npc::inrange(Point<int16_t> cursorpos, Point<int16_t> viewpos) const
	{
		if (!active)
			return false;

		Point<int16_t> absp = getposition() + viewpos;
		Point<int16_t> dim = animations.count(stance) ?
			animations.at(stance).getdimensions() :
			Point<int16_t>();

		return rectangle2d<int16_t>(
			absp.x() - dim.x() / 2, 
			absp.x() + dim.x() / 2, 
			absp.y() - dim.y(), 
			absp.y()
			).contains(cursorpos);
	}
}
