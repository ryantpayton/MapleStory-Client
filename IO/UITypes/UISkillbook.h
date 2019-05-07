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
#pragma once
#include "../UIElement.h"
#include "../UIDragElement.h"

#include "../Components/Slider.h"

#include "../../Character/CharStats.h"
#include "../../Character/Skillbook.h"
#include "../../Graphics/Text.h"

#include <vector>

namespace jrc
{
	class SkillIcon
	{
	public:
		SkillIcon(int32_t id, int32_t level);

		void draw(const DrawArgument& args) const;

		Cursor::State send_cursor(Point<int16_t> cursorpos, bool clicked);

		enum State
		{
			NORMAL,
			DISABLED,
			MOUSEOVER
		};

		void set_state(State state);

		int32_t get_id() const;
		int32_t get_level() const;

	private:
		Texture normal;
		Texture disabled;
		Texture mouseover;
		Text name;
		Text level;
		int32_t id;
		int32_t lv;

		State state;
		bool enabled;
	};


	class UISkillbook : public UIDragElement<PosSKILL>
	{
	public:
		static constexpr Type TYPE = SKILLBOOK;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UISkillbook(const CharStats& stats, const Skillbook& skillbook);

		void draw(float alpha) const override;

		void toggle_active() override;
		void doubleclick(Point<int16_t> cursorpos) override;
		bool remove_cursor(bool clicked, Point<int16_t> cursorpos) override;
		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;
		void send_key(int32_t keycode, bool pressed) override;

		void update_stat(Maplestat::Id stat, int16_t value);
		void update_skills(int32_t skill_id);

	protected:
		Button::State button_pressed(uint16_t id) override;

	private:
		void change_job(uint16_t id);
		void change_sp(int16_t value);
		void change_beginner_sp();
		void change_tab(uint16_t new_tab);
		void change_offset(uint16_t new_offset);

		void show_skill(int32_t skill_id);
		void clear_tooltip();

		bool can_raise(int32_t skill_id) const;
		void send_spup(uint16_t row);

		Job::Level joblevel_by_tab(uint16_t tab) const;
		SkillIcon* icon_by_position(Point<int16_t> cursorpos);

		void close();
		int16_t calculate_remaining_beginner_sp();
		bool check_required(int32_t id) const;

		enum Buttons : uint16_t
		{
			BT_CLOSE,
			BT_HYPER,
			BT_GUILD,
			BT_MOUNT,
			BT_MACRO,
			BT_TAB0,
			BT_TAB1,
			BT_TAB2,
			BT_TAB3,
			BT_TAB4,
			BT_SPUP0,
			BT_SPUP1,
			BT_SPUP2,
			BT_SPUP3,
			BT_SPUP4,
			BT_SPUP5,
			BT_SPUP6,
			BT_SPUP7,
			BT_SPUP8,
			BT_SPUP9,
			BT_SPUP10,
			BT_SPUP11
		};

		static constexpr int16_t ROWS = 12;
		static constexpr int16_t ROW_HEIGHT = 40;
		static constexpr int16_t ROW_WIDTH = 143;
		static constexpr Point<int16_t> SKILL_OFFSET = Point<int16_t>(11, 93);
		static constexpr Point<int16_t> ICON_OFFSET = Point<int16_t>(2, 34);
		static constexpr Point<int16_t> LINE_OFFSET = Point<int16_t>(0, 37);

		const CharStats& stats;
		const Skillbook& skillbook;

		Slider slider;
		Texture skille;
		Texture skilld;
		Texture skillb;
		Texture line;
		Texture bookicon;
		Text booktext;
		Text splabel;

		Job job;
		int16_t sp;
		int16_t beginner_sp;

		uint16_t tab;
		uint16_t skillcount;
		uint16_t offset;

		std::vector<SkillIcon> icons;
		bool grabbing;
	};
}