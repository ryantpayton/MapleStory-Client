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
#include "Skill.h"

#include "Util\Misc.h"

#include "nlnx\node.hpp"
#include "nlnx\nx.hpp"

namespace Gameplay
{
	Skill::Skill(int32_t id)
	{
		string strid = Format::extendid(id, 7);
		node src = nl::nx::skill[strid.substr(0, 3) + ".img"]["skill"][strid];

		skillid = id;

		icons[NORMAL] = src["icon"];
		icons[DISABLED] = src["iconDisabled"];
		icons[MOUSEOVER] = src["iconMouseOver"];

		oneuseeffect = src["effect"]["0"].data_type() == node::type::bitmap;
		if (oneuseeffect)
		{
			useeffects[false] = src["effect"];
		}
		else
		{
			useeffects[false] = src["effect"]["0"];
			useeffects[true] = src["effect"]["1"];
		}

		onehiteffect = src["hit"]["0"].data_type() == node::type::bitmap;
		if (onehiteffect)
		{
			hiteffects[false] = src["hit"];
		}
		else
		{
			hiteffects[false] = src["hit"]["0"];
			hiteffects[true] = src["hit"]["1"];
		}

		actions[false] = src["action"]["0"];
		actions[true] = src["action"]["1"];
		oneaction = actions[true] == "";

		affected = src["affected"];

		preparestance = src["prepare"]["action"];
		preparetime = src["prepare"]["time"];

		offensive = false;
		for (node leveln : src["level"])
		{
			Level level;
			if (leveln["damage"].data_type() == node::type::integer)
			{
				offensive = true;

				level.damage = leveln["damage"];
				level.damage /= 100;
				level.attackcount = leveln["attackCount"];
				if (level.attackcount == 0)
					level.attackcount = 1;
				level.mobcount = leveln["mobCount"];
				if (level.mobcount == 0)
					level.mobcount = 1;

				if (attackframes.count(skillid))
				{
					for (auto frameit : attackframes[skillid])
					{
						bool twohanded = frameit.first;
						for (int16_t frame : frameit.second)
						{
							uint16_t delay = useeffects[twohanded].getdelayuntil(frame);
							level.hitdelays[twohanded].push_back(delay);
						}
					}
				}
			}
			level.chance = leveln["prop"];
			if (level.chance <= 0.0f)
				level.chance = 100;
			level.chance /= 100;
			level.hpcost = leveln["hpCon"];
			level.mpcost = leveln["mpCon"];
			level.range = rectangle2d<int16_t>(leveln["lt"], leveln["rb"]);

			int32_t lvlid = std::stoi(leveln.name());
			levels[lvlid] = level;
		}
	}

	Skill::Skill() {}

	Skill::~Skill() {}

	bool Skill::isoffensive() const
	{
		return offensive;
	}

	bool Skill::canbetwohanded() const
	{
		return !onehiteffect;
	}

	int32_t Skill::getid() const
	{
		return skillid;
	}

	string Skill::getaction(bool twohanded) const
	{
		bool second = twohanded && !oneaction;
		return actions[second];
	}

	Animation Skill::gethitanimation(bool twohanded) const
	{
		bool second = twohanded && !onehiteffect;
		return hiteffects[second];
	}

	Animation Skill::geteffect(bool twohanded) const
	{
		bool second = twohanded && !oneuseeffect;
		return useeffects[second];
	}

	const Texture& Skill::geticon(IconType type) const
	{
		return icons.at(type);
	}

	const Skill::Level* Skill::getlevel(int32_t level) const
	{
		return levels.count(level) ? &levels.at(level) : nullptr;
	}


	void Skill::init()
	{
		attackframes[BRANDISH][true] = { 3, 5 };
		attackframes[BRANDISH][false] = { 2, 6 };
	}
	unordered_map<int32_t, BoolPair<vector<int16_t>>> Skill::attackframes;
}