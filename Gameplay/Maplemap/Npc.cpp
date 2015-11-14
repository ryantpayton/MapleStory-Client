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
	Npc::Npc(int32_t id, int32_t o, bool fl, uint16_t f, bool c, int32_t x, int32_t y)
	{
		string strid = std::to_string(id);
		strid.insert(0, 7 - strid.size(), '0');
		strid.append(".img");

		using::nl::node;
		node src = nl::nx::npc[strid];
		node strsrc = nl::nx::string["Npc.img"][std::to_string(id)];

		string link = src["info"]["link"];
		if (link.size() > 0)
		{
			link.append(".img");
			src = nl::nx::npc[link];
		}

		for (node& npcnode : src)
		{
			string state = npcnode.name();
			if (state == "info")
			{
				hidename = npcnode["hideName"].get_bool();
				mouseonly = npcnode["talkMouseOnly"].get_bool();
				scripted = npcnode["script"].size() > 0;
			}
			else
			{
				animations[state] = Animation(npcnode);
				states.push_back(state);
			}

			for (node& speaknode : npcnode["speak"])
			{
				lines[state].push_back(strsrc[speaknode.get_string()]);
			}
		}

		name = strsrc["name"];
		func = strsrc["func"];

		namelabel = Textlabel(Textlabel::DWF_14BC, Textlabel::TXC_YELLOW, name, 0);
		funclabel = Textlabel(Textlabel::DWF_14BC, Textlabel::TXC_YELLOW, func, 0);
		namelabel.setback(Textlabel::TXB_NAMETAG);
		funclabel.setback(Textlabel::TXB_NAMETAG);

		npcid = id;
		oid = o;
		flip = !fl;
		stance = "stand";
		active = true;
		control = c;

		phobj.fhid = f;
		setposition(x, y);
	}

	void Npc::draw(vector2d<int32_t> viewpos, float inter) const
	{
		if (active)
		{
			vector2d<int32_t> absp = phobj.getposition(inter) + viewpos;

			if (animations.count(stance))
			{
				using::Graphics::DrawArgument;
				animations.at(stance).draw(DrawArgument(absp, flip), inter);
			}
			if (!hidename)
			{
				namelabel.draw(absp);
				funclabel.draw(absp + vector2d<int32_t>(0, 18));
			}
		}
	}

	int8_t Npc::update(const Physics& physics)
	{
		if (active)
		{
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

	void Npc::setactive(bool a)
	{
		active = a;
	}

	void Npc::setposition(int32_t x, int32_t y)
	{
		phobj.fx = static_cast<float>(x);
		phobj.fy = static_cast<float>(y);
	}

	int32_t Npc::getoid() const
	{
		return oid;
	}

	int8_t Npc::getlayer() const
	{
		return phobj.fhlayer;
	}

	vector2d<int32_t> Npc::getposition() const
	{
		return vector2d<int32_t>(
			static_cast<int32_t>(phobj.fx), 
			static_cast<int32_t>(phobj.fy)
			);
	}

	vector2d<int32_t> Npc::getdimensions() const
	{
		if (animations.count(stance))
			return animations.at(stance).getdimensions();
		else
			return vector2d<int32_t>();
	}
}
