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

#include "../UIElement.h"

#include "../Components/Textfield.h"

#include "../../Template/BoolPair.h"

namespace ms
{
	// Race selection screen
	class UIRaceSelect : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::RACESELECT;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = false;

		UIRaceSelect();

		void draw(float inter) const override;
		void update() override;

		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;
		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

		bool check_name(std::string name) const;
		void send_naming_result(bool nameused);

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void select_class(uint16_t index);
		void show_charselect();
		void show_worldselect();
		Point<int16_t> get_class_pos(size_t index) const;
		std::string to_lower(std::string value) const;
		uint16_t get_corrected_class_index(uint16_t index) const;

		static constexpr uint16_t INDEX_COUNT = 6;
		static constexpr uint16_t SELECTED_LIST = 35;

		enum Buttons : uint16_t
		{
			BtStart,
			BtPreview,
			BtMake,
			BtLeftArrow,
			BtRightArrow,
			CLASS0,
			CLASS1,
			CLASS2,
			CLASS3,
			CLASS4
		};

		enum Classes : uint16_t
		{
			RESISTANCE,
			EXPLORERS,
			CYGNUSKNIGHTS,
			ARAN,
			EVAN,
			MERCEDES,
			DEMON,
			PHANTOM,
			DUALBLADE,
			MIHILE,
			LUMINOUS,
			KAISER,
			ANGELICBUSTER,
			CANNONEER,
			XENON,
			ZERO,
			SHADE,
			PINKBEAN,
			KINESIS,
			CADENA,
			ILLIUM,
			ARK,
			PATHFINDER,
			HOYOUNG,
			ADELE,
			KAIN,
			YETI,
			LARA,
			JETT = 1000,
			HAYATO,
			KANNA,
			CHASE
		};

		Text version;
		Point<int16_t> version_pos;
		Point<int16_t> pos;
		Point<int16_t> posZero;
		std::vector<uint16_t> order;
		nl::node hotlist;
		nl::node newlist;
		nl::node bgm;
		nl::node Back2;
		Sprite hotlabel;
		Sprite hotlabelZero;
		Sprite newlabel;
		Point<int16_t> newlabelPos;
		Sprite hotbtn;
		Point<int16_t> hotbtnPos;
		Sprite newbtn;
		Point<int16_t> newbtnPos;
		uint16_t class_index[INDEX_COUNT];
		bool mouseover[INDEX_COUNT];
		uint16_t selected_class;
		uint16_t index_shift;
		uint16_t selected_index;
		uint16_t class_count;
		std::vector<bool> class_isdisabled;
		std::vector<BoolPair<Texture>> class_disabled;
		std::vector<BoolPair<Texture>> class_normal;
		std::vector<Texture> class_background;
		std::vector<Texture> class_details;
		std::vector<Texture> class_title;
		std::vector<uint16_t> class_map;
		Texture back;
		Texture backZero;
		Sprite back_ani;
		Texture class_details_background;
		Texture class_details_backgroundZero;
		Texture dot;
		int16_t buttonIntervalX;
		Point<int16_t> buttonPos;
	};
}