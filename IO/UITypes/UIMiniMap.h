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

#include "../UIDragElement.h"

#include "../../Graphics/Text.h"
#include "../../Character/Charstats.h"

namespace ms
{
	class UIMiniMap : public UIDragElement<PosMINIMAP>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::MINIMAP;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIMiniMap(const CharStats& stats);

		void draw(float alpha) const override;
		void update() override;

		void send_key(std::int32_t keycode, bool pressed, bool escape) override;

	private:
		static constexpr std::int16_t center_start_x = 64;
		const CharStats& stats;

		void update_buttons();
		void toggle_buttons();
		void update_text();

		enum Buttons
		{
			BT_MAP,
			BT_MAX,
			BT_MIN,
			BT_NPC,
			BT_SMALL
		};

		enum Type
		{
			MIN,
			NORMAL,
			MAX
		};

		std::int32_t mapid;
		std::int8_t type;
		std::int16_t combined_text_width;
		Point<int16_t> bt_map_pos;
		Point<int16_t> bt_max_pos;
		Point<int16_t> bt_min_pos;
		std::int16_t bt_min_width;
		std::int16_t bt_max_width;
		std::int16_t bt_map_width;
		std::int16_t final_pos;
		Animation min_left;
		Animation min_center;
		Animation min_right;
		std::vector<Sprite> normal_sprites;
		std::vector<Sprite> max_sprites;
		Text combined_text;
		Text region_text;
		Text town_text;
	};
}