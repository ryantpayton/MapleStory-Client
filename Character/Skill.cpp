/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
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
#include "nlnx\node.hpp"
#include "nlnx\nx.hpp"

namespace Character
{
	Skill::Skill(int32_t id)
	{
		string strid = std::to_string(id);
		strid.insert(0, 7 - strid.length(), '0');
		nl::node src = nl::nx::skill[strid.substr(0, 3) + ".img"]["skill"][strid];

		icons[NORMAL] = Texture(src["icon"]);
		icons[DISABLED] = Texture(src["iconDisabled"]);
		icons[MOUSEOVER] = Texture(src["iconMouseOver"]);

		if (src["effect"]["0"].data_type() == nl::node::type::bitmap)
		{
			effects.push_back(Animation(src["effect"]));
		}
		else
		{
			for (auto effectn : src["effect"])
			{
				effects.push_back(Animation(effectn));
			}
		}

		if (src["hit"]["0"].data_type() == nl::node::type::bitmap)
		{
			hit.push_back(Animation(src["hit"]));
		}
		else
		{
			for (auto effectn : src["hit"])
			{
				hit.push_back(Animation(effectn));
			}
		}

		affected = Animation(src["affected"]);

		preparestance = src["prepare"]["action"];
		preparetime = src["prepare"]["time"];
		offensive = src["level"]["1"]["damage"].data_type() == nl::node::type::integer;

		for (auto actionn : src["action"])
		{
			actions.push_back(actionn);
		}

		for (auto leveln : src["level"])
		{
			int32_t lvlid = std::stoi(leveln.name());
			SkillLevel level;
			if (leveln["prop"].data_type() == nl::node::type::integer)
				level.chance = static_cast<float>(leveln["prop"]) / 100.0f;
			else
				level.chance = 1.0f;
			if (offensive)
			{
				level.damage = static_cast<float>(leveln["damage"]) / 100.0f;
				level.attackcount = static_cast<uint8_t>(leveln["attackCount"]);
				if (level.attackcount == 0)
					level.attackcount = 1;
				level.mobcount = static_cast<uint8_t>(leveln["mobCount"]);
				if (level.mobcount == 0)
					level.mobcount = 1;
			}
			level.hpcost = leveln["hpCon"];
			level.mpcost = leveln["mpCon"];
			level.range = rectangle2d<int16_t>(
				vector2d<int16_t>(leveln["lt"]),
				vector2d<int16_t>(leveln["rb"])
				);
			levels[lvlid] = level;
		}
	}

	Skill::Skill() {}

	Skill::~Skill() {}

	bool Skill::isoffensive() const
	{
		return offensive;
	}

	int32_t Skill::getid() const
	{
		return id;
	}

	string Skill::getaction(bool twohanded) const
	{
		if (actions.size() == 0)
			return "";

		if (actions.size() > 1 && twohanded)
			return actions[1];
		else
			return actions[0];
	}

	Optional<SkillLevel> Skill::getlevel(int32_t level) const
	{
		if (levels.count(level))
			return levels.at(level);
		else
			return nullptr;
	}

	Animation Skill::gethitanimation(bool twohanded) const
	{
		if (effects.size() == 0)
			return Animation();

		if (effects.size() > 1 && twohanded)
			return hit[1];
		else
			return hit[0];
	}

	Animation Skill::geteffect(bool twohanded) const
	{
		if (effects.size() == 0)
			return Animation();

		if (effects.size() > 1 && twohanded)
			return effects[1];
		else
			return effects[0];
	}

	const Texture& Skill::geticon(IconType type) const
	{
		return icons.at(type);
	}
}