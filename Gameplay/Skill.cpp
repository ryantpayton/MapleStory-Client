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
#include "nlnx\node.hpp"
#include "nlnx\nx.hpp"

namespace Gameplay
{
	Skill::Skill(int32_t id)
	{
		string strid = std::to_string(id);
		strid.insert(0, 7 - strid.length(), '0');
		nl::node src = nl::nx::skill[strid.substr(0, 3) + ".img"]["skill"][strid];

		icons[0] = Texture(src["icon"]);
		icons[1] = Texture(src["iconDisabled"]);
		icons[2] = Texture(src["iconMouseOver"]);

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

		hit = Animation(src["hit"]);
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
			uint8_t lvlid = static_cast<uint8_t>(std::stoi(leveln.name()));
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

	string Skill::getaction(bool twohanded) const
	{
		if (actions.size() == 0)
			return "";

		if (actions.size() > 1 && twohanded)
			return actions[1];
		else
			return actions[0];
	}
}