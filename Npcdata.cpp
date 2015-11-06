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
#include "Npcdata.h"
#include "nx.hpp"

namespace Data
{
	Npcdata::Npcdata(int32_t id)
	{
		string strid = std::to_string(id);
		strid.insert(0, 7 - strid.size(), '0');
		strid.append(".img");

		using::nl::node;
		node src = nl::nx::npc[strid];

		string link = src["info"]["link"];
		if (link.size() > 0)
		{
			link.append(".img");
			src = nl::nx::npc[link];
		}

		map<string, vector<string>> linenames;
		for (node npcnode = src.begin(); npcnode != src.end(); ++npcnode)
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
				animations.add(state, new AnimationData(npcnode));
				states.push_back(state);
			}

			node speak = npcnode["speak"];
			if (speak.size() > 0)
			{
				for (node speaknode = speak.begin(); speaknode != speak.end(); speaknode++)
				{
					linenames[state].push_back(speaknode.get_string());
				}
			}
		}

		node strsrc = nl::nx::string["Npc.img"][std::to_string(id)];

		name = strsrc["name"];
		func = strsrc["func"];

		for (map<string, vector<string>>::iterator stit = linenames.begin(); stit != linenames.end(); ++stit)
		{
			string state = stit->first;
			vector<string> names = stit->second;
			for (vector<string>::iterator nit = names.begin(); nit != names.end(); ++nit)
			{
				lines[state].push_back(strsrc[*nit]);
			}
		}
	}

	Npcdata::~Npcdata() {}

	bool Npcdata::showname() const
	{
		return !hidename;
	}

	bool Npcdata::isscripted() const
	{
		return scripted;
	}

	bool Npcdata::ismouseonly() const
	{
		return mouseonly;
	}

	size_t Npcdata::getnumstates() const
	{
		return states.size();
	}

	string Npcdata::getname() const
	{
		return name;
	}

	string Npcdata::getfunc() const
	{
		return func;
	}

	string Npcdata::getstate(size_t s) const
	{
		return (s < states.size()) ? states.at(s) : "";
	}

	vector2d<int32_t> Npcdata::getdimensions(string st, uint8_t frame) const
	{
		AnimationData* anim = animations.get(st);
		if (anim != 0)
		{
			return anim->getdimensions(frame);
		}
		else
		{
			return vector2d<int32_t>();
		}
	}

	const Texture* Npcdata::gettexture(string st, uint8_t frame) const
	{
		AnimationData* anim = animations.get(st);
		if (anim)
		{
			return anim->gettexture(frame);
		}
		else
		{
			return nullptr;
		}
	}

	const AnimationData& Npcdata::getanimation(string st) const
	{
		const AnimationData* anim = animations.get(st);
		if (anim)
		{
			return *anim;
		}
		else
		{
			return defaultanim;
		}
	}
}