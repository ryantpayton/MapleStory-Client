//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "SkillTooltip.h"

#include "../../Data/SkillData.h"

#include "nlnx/nx.hpp"
#include "nlnx/node.hpp"

namespace jrc
{
	SkillTooltip::SkillTooltip()
		: line(318, Geometry::WHITE, 1.0f) {

		nl::node itemtt = nl::nx::ui["UIToolTip.img"]["Item"];

		frame = itemtt["Frame2"];
		base = itemtt["ItemIcon"]["base"];
		cover = itemtt["ItemIcon"]["cover"];

		skill_id = 0;
	}

	void SkillTooltip::set_skill(int32_t id, int32_t level, 
		int32_t mlevel, int64_t expiration) {

		if (skill_id == id)
			return;

		skill_id = id;

		if (skill_id == 0)
			return;

		const SkillData& data = SkillData::get(id);

		int32_t masterlevel;
		if (mlevel > 0)
		{
			masterlevel = mlevel;
		}
		else
		{
			masterlevel = data.get_masterlevel();
		}

		std::string descstr = data.get_desc();
		if (masterlevel > 0)
		{
			const std::string mltag = "Master Level";
			const std::string mlstr = std::to_string(masterlevel);
			size_t mlstart = descstr.find(mltag);
			size_t mlpos = descstr.find(':', mlstart) + 2;
			size_t mlend = descstr.find("]", mlstart);
			if (mlpos < mlend && mlend != std::string::npos)
			{
				size_t mlsize = mlend - mlpos;
				descstr.erase(mlpos, mlsize);
				descstr.insert(mlpos, mlstr);

				// fixing errors in the files...
				if (mlstart == 0)
				{
					descstr.insert(0, "[");
					mlend++;
				}
				size_t linebreak = descstr.find("]\\n", mlstart);
				if (linebreak != mlend)
				{
					descstr.insert(mlend + 1, "\\n");
				}
			}
			else
			{
				descstr.insert(0, "[" + mltag + ": " + mlstr + "]\\n");
			}
		}

		const std::string exptag = "#cAvailable until";
		if (expiration > 0)
		{
			// TODO
		}
		else
		{
			size_t expstart = descstr.find(exptag);
			size_t expend = descstr.find('#', expstart + 1);
			if (expstart < expend && expend != std::string::npos)
			{
				size_t expsize = expend - expstart + 1;
				descstr.erase(expstart, expsize);
			}
		}

		if (data.is_passive())
		{
			descstr += "\\r#cPassive Skill#";
		}

		std::string levelstr;
		bool current = level > 0;
		bool next = level < masterlevel;
		if (current)
		{
			levelstr += "[Current Level: " + std::to_string(level) + "]\\n"
				+ data.get_level_desc(level);
		}
		if (current && next)
		{
			levelstr += "\\n";
		}
		if (next)
		{
			levelstr += "[Next Level: " + std::to_string(level + 1) + "]\\n"
				+ data.get_level_desc(level + 1);
		}

		icon = data.get_icon(SkillData::NORMAL);
		name = { Text::A12B, Text::LEFT, Text::WHITE, data.get_name(), 320 };
		desc = { Text::A12M, Text::LEFT, Text::WHITE, descstr, 230 };
		leveldesc = { Text::A12M, Text::LEFT, Text::WHITE, levelstr, 330 };

		icon_offset = 4 + name.height();
		level_offset = std::max<int16_t>(desc.height(), 92) + 16;
		height = icon_offset + level_offset + leveldesc.height();
	}

	void SkillTooltip::draw(Point<int16_t> pos) const
	{
		if (skill_id == 0)
			return;

		frame.draw(pos + Point<int16_t>(176, height + 16), 320, height);
		name.draw(pos + Point<int16_t>(16, 8));

		pos.shift_y(icon_offset);

		base.draw({ pos + Point<int16_t>(12, 16) });
		icon.draw({ pos + Point<int16_t>(22, 90), 2.0f, 2.0f });
		cover.draw({ pos + Point<int16_t>(12, 16) });

		desc.draw(pos + Point<int16_t>(102, 12));

		pos.shift_y(level_offset);

		line.draw(pos + Point<int16_t>(14, 4));
		leveldesc.draw(pos + Point<int16_t>(12, 12));
	}
}