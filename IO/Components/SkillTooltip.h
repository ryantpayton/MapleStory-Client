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
#pragma once

#include "Tooltip.h"
#include "MapleFrame.h"

#include "../Graphics/Geometry.h"

namespace ms
{
	class SkillTooltip : public Tooltip
	{
	public:
		SkillTooltip();

		void draw(Point<int16_t> position) const override;

		void set_skill(int32_t id, int32_t level, int32_t masterlevel, int64_t expiration);

	private:
		int32_t skill_id;
		int16_t height;
		int16_t width;
		int16_t icon_offset;
		int16_t level_offset;
		Texture icon;
		Texture required_icon;

		Text name;
		Text desc;
		Text leveldesc;
		MapleFrame frame;
		ColorLine line;
		ColorBox box;
		Texture cover;
	};
}