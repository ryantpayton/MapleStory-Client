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
#include "UIMiniMap.h"

#include "../Components/MapleButton.h"

#include <nlnx/nx.hpp>

namespace ms
{
	UIMiniMap::UIMiniMap(const CharStats& st) : UIDragElement<PosMINIMAP>(Point<int16_t>(50, 50)), stats(st)
	{
		type = Type::MIN;
		bool simpleMode = false;

		std::string node = simpleMode ? "MiniMapSimpleMode" : "MiniMap";
		nl::node MiniMap = nl::nx::ui["UIWindow2.img"][node];

		nl::node Min, Normal, Max, Map;

		if (simpleMode)
		{
			Min = MiniMap["Window"]["Min"];
			Normal = MiniMap["Window"]["Normal"];
			Max = MiniMap["Window"]["Max"];
		}
		else
		{
			Min = MiniMap["Min"];
			Normal = MiniMap["MinMap"];
			Max = MiniMap["MaxMap"];
		}


		mapid = stats.get_mapid();
		Map = get_map_node_name();
		Sprite map_sprite = Sprite(Map);
		Point<int16_t> map_dimensions = map_sprite.get_animation().get_dimensions();

		int16_t map_x = 13;
		int16_t window_ur_x_offset = map_dimensions.x() - 128 + map_x * 2;

		std::string Left = simpleMode ? "Left" : "w";
		std::string Center = simpleMode ? "Center" : "c";
		std::string Right = simpleMode ? "Right" : "e";

		bt_map_pos = Point<int16_t>(237, -6);
		bt_max_pos = Point<int16_t>(209, -6);
		bt_min_pos = Point<int16_t>(195, -6);

		buttons[Buttons::BT_MAP] = std::make_unique<MapleButton>(MiniMap["BtMap"], bt_map_pos);
		buttons[Buttons::BT_MAX] = std::make_unique<MapleButton>(MiniMap["BtMax"], bt_max_pos);
		buttons[Buttons::BT_MIN] = std::make_unique<MapleButton>(MiniMap["BtMin"], bt_min_pos);
		buttons[Buttons::BT_NPC] = std::make_unique<MapleButton>(MiniMap["BtNpc"], Point<int16_t>(276, -6));
		buttons[Buttons::BT_SMALL] = std::make_unique<MapleButton>(MiniMap["BtSmall"], Point<int16_t>(223, -6));

		min_left = Min[Left];
		min_center = Min[Center];
		min_right = Min[Right];

		combined_text = Text(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::WHITE);

		std::string DownCenter = simpleMode ? "DownCenter" : "s";
		std::string DownLeft = simpleMode ? "DownLeft" : "sw";
		std::string DownRight = simpleMode ? "DownRight" : "se";
		std::string MiddleCenter = "MiddleCenter";
		std::string MiddleLeft = simpleMode ? "MiddleLeft" : "w";
		std::string MiddleRight = simpleMode ? "MiddleRight" : "e";
		std::string UpCenter = simpleMode ? "UpCenter" : "n";
		std::string UpLeft = simpleMode ? "UpLeft" : "nw";
		std::string UpRight = simpleMode ? "UpRight" : "ne";

		Point<int16_t> m_stretch = Point<int16_t>(0, map_dimensions.y() - 17);
		DrawArgument uc_pos = DrawArgument(window_ul_pos) + DrawArgument(Point<int16_t>(64, 0), Point<int16_t>(window_ur_x_offset, 0));
		DrawArgument mr_pos = DrawArgument(Point<int16_t>(window_ur_x_offset + 64 + 55, 0));
		
		Point<int16_t> ur_pos = Point<int16_t>(64 + window_ur_x_offset, -10);

		int16_t ml_mr_y = 17;
		int16_t dl_dr_y = map_dimensions.y();
		int16_t dc_y = dl_dr_y + 18;
		
		normal_sprites.emplace_back(Normal[UpLeft], window_ul_pos);
		normal_sprites.emplace_back(Normal[UpCenter], uc_pos);
		normal_sprites.emplace_back(Normal[UpRight], ur_pos);
		normal_sprites.emplace_back(Normal[MiddleCenter]);
		normal_sprites.emplace_back(Normal[MiddleLeft], DrawArgument(Point<int16_t>(0, ml_mr_y), m_stretch));
		normal_sprites.emplace_back(Normal[MiddleRight], mr_pos + DrawArgument(Point<int16_t>(0, ml_mr_y), m_stretch));
		normal_sprites.emplace_back(Normal[DownCenter], DrawArgument(Point<int16_t>(0, dc_y)) + DrawArgument(Point<int16_t>(64, 0), Point<int16_t>(window_ur_x_offset, 0)));
		normal_sprites.emplace_back(Normal[DownLeft], Point<int16_t>(0, dl_dr_y));
		normal_sprites.emplace_back(Normal[DownRight], Point<int16_t>(64 + window_ur_x_offset, dl_dr_y));
		
		normal_sprites.emplace_back(Map, DrawArgument(position + Point<int16_t>(map_x, ml_mr_y - 5)));

		int16_t max_adj = 40;
		int16_t max_dc_y = dc_y + max_adj;
		int16_t max_dl_dr_y = dl_dr_y + max_adj;
		int16_t max_ml_mr_y = ml_mr_y + max_adj;

		max_sprites.emplace_back(Max[DownCenter], DrawArgument(Point<int16_t>(0, max_dc_y)) + DrawArgument(Point<int16_t>(64, 0), Point<int16_t>(window_ur_x_offset, 0)));
		max_sprites.emplace_back(Max[DownLeft], Point<int16_t>(0, max_dl_dr_y));
		max_sprites.emplace_back(Max[DownRight], Point<int16_t>(window_ur_x_offset + 64, max_dl_dr_y));
		max_sprites.emplace_back(Max[MiddleCenter]);
		max_sprites.emplace_back(Max[MiddleLeft], DrawArgument(Point<int16_t>(0, max_ml_mr_y), m_stretch));
		max_sprites.emplace_back(Max[MiddleRight], mr_pos + DrawArgument(Point<int16_t>(0, max_ml_mr_y), m_stretch));
		max_sprites.emplace_back(Max[UpCenter], uc_pos);
		max_sprites.emplace_back(Max[UpLeft], window_ul_pos);
		max_sprites.emplace_back(Max[UpRight], ur_pos);
		max_sprites.emplace_back(Map, DrawArgument(position + Point<int16_t>(map_x, max_ml_mr_y - 5)));

		region_text = Text(Text::Font::A12B, Text::Alignment::LEFT, Color::Name::WHITE);
		town_text = Text(Text::Font::A12B, Text::Alignment::LEFT, Color::Name::WHITE);
	}

	void UIMiniMap::draw(float alpha) const
	{
		if (type == Type::MIN)
		{
			DrawArgument min_left_pos = Point<int16_t>(0, -10);

			min_left.draw(min_left_pos, alpha);
			min_center.draw(min_left_pos + DrawArgument(Point<int16_t>(center_start_x, 0), Point<int16_t>(final_pos - center_start_x, 0)), alpha);
			min_right.draw(Point<int16_t>(final_pos, -10), alpha);

			combined_text.draw(position + Point<int16_t>(7, -13));
		}
		else if (type == Type::NORMAL)
		{
			for each (Sprite sprite in normal_sprites)
				sprite.draw(position, alpha);
		}
		else
		{
			for each (Sprite sprite in max_sprites)
				sprite.draw(position, alpha);

			region_text.draw(position + Point<int16_t>(48, 4));
			town_text.draw(position + Point<int16_t>(48, 18));
		}

		UIElement::draw(alpha);
	}

	void UIMiniMap::update()
	{
		int32_t mid = stats.get_mapid();

		if (mid != mapid)
		{
			mapid = mid;
			update_canvas();
			update_text();
			update_buttons();
			toggle_buttons();
		}

		if (type == Type::MIN)
		{
			min_left.update();
			min_center.update();
			min_right.update();
		}
		else if (type == Type::NORMAL)
		{
			for each (Sprite sprite in normal_sprites)
				sprite.update();
		}
		else
		{
			for each (Sprite sprite in max_sprites)
				sprite.update();
		}

		UIElement::update();
	}

	void UIMiniMap::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (type < Type::MAX)
			type++;
		else
			type = Type::MIN;

		toggle_buttons();
	}

	void UIMiniMap::update_buttons()
	{
		// Add one pixel for a space to the right of each button
		bt_min_width = buttons[Buttons::BT_MIN]->width() + 1;
		bt_max_width = buttons[Buttons::BT_MAX]->width() + 1;
		bt_map_width = buttons[Buttons::BT_MAP]->width() + 1;
		combined_text_width = combined_text.width() / 2;
	}

	void UIMiniMap::toggle_buttons()
	{
		if (type == Type::MIN)
		{
			buttons[Buttons::BT_MAP]->set_active(true);
			buttons[Buttons::BT_MAX]->set_active(true);
			buttons[Buttons::BT_MIN]->set_active(true);
			buttons[Buttons::BT_NPC]->set_active(false);
			buttons[Buttons::BT_SMALL]->set_active(false);

			buttons[Buttons::BT_MIN]->set_state(Button::State::DISABLED);
			buttons[Buttons::BT_MAX]->set_state(Button::State::NORMAL);

			int16_t bt_min_x = position.x() + combined_text_width + center_start_x + bt_min_width + bt_max_width + bt_map_width;
			final_pos = bt_min_x + bt_min_width - 2;

			buttons[Buttons::BT_MIN]->set_position(Point<int16_t>(bt_min_x, bt_min_pos.y()));

			int16_t bt_max_x = bt_min_x + bt_max_width;

			buttons[Buttons::BT_MAX]->set_position(Point<int16_t>(bt_max_x, bt_max_pos.y()));

			int16_t bt_map_x = bt_max_x + bt_max_width;

			buttons[Buttons::BT_MAP]->set_position(Point<int16_t>(bt_map_x, bt_map_pos.y()));
		}
		else
		{
			buttons[Buttons::BT_MAP]->set_active(true);
			buttons[Buttons::BT_MAX]->set_active(true);
			buttons[Buttons::BT_MIN]->set_active(true);
			buttons[Buttons::BT_NPC]->set_active(true);
			buttons[Buttons::BT_SMALL]->set_active(true);

			buttons[Buttons::BT_MIN]->set_state(Button::State::NORMAL);

			if (type == Type::MAX)
				buttons[Buttons::BT_MAX]->set_state(Button::State::DISABLED);
			else
				buttons[Buttons::BT_MAX]->set_state(Button::State::NORMAL);

			buttons[Buttons::BT_MIN]->set_position(bt_min_pos);
			buttons[Buttons::BT_MAX]->set_position(bt_max_pos);
			buttons[Buttons::BT_MAP]->set_position(bt_map_pos);
		}
	}

	void UIMiniMap::update_text()
	{
		nl::node Map = nl::nx::string["Map.img"];
		nl::node foundMap;

		for (auto t = Map.begin(); t != Map.end(); ++t)
		{
			foundMap = t.resolve(std::to_string(mapid));

			if (foundMap.size() > 0)
			{
				std::string mapName = foundMap["mapName"];
				std::string streetName = foundMap["streetName"];
				std::string combined = mapName + ": " + streetName;

				combined_text.change_text(combined);
				region_text.change_text(mapName);
				town_text.change_text(streetName);
				break;
			}
		}
	}

	void UIMiniMap::update_canvas()
	{
		// Delete old map canvas
		normal_sprites.pop_back();
		max_sprites.pop_back();

		nl::node map = get_map_node_name();

		// Emplace new canvas into sprite vectors
		normal_sprites.emplace_back(map, DrawArgument(position + Point<int16_t>(3, 17)));
		max_sprites.emplace_back(map, DrawArgument(position + Point<int16_t>(3, 57)));
	}

	nl::node UIMiniMap::get_map_node_name() {
		// Create node name from new map id
		std::string mid_string = "000000000.img";
		std::string id_string = std::to_string(mapid);
		mid_string.replace(9 - id_string.length(), id_string.length(), id_string);

		// Get canvas of new map
		return nl::nx::map["Map"]["Map" + std::to_string(mapid / 100000000)][mid_string]["miniMap"]["canvas"];
	}
}