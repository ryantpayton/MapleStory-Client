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
#include "UIStatsinfo.h"

#include "../UI.h"

#include "../Components/MapleButton.h"
#include "../UITypes/UINotice.h"

#include "../../Net/Packets/PlayerPackets.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UIStatsinfo::UIStatsinfo(const CharStats& st) : UIDragElement<PosSTATS>(Point<int16_t>(212, 20)), stats(st)
	{
		nl::node close = nl::nx::ui["Basic.img"]["BtClose"];
		nl::node Stat = nl::nx::ui["UIWindow4.img"]["Stat"];
		nl::node main = Stat["main"];
		nl::node detail = Stat["detail"];
		nl::node abilityTitle = detail["abilityTitle"];
		nl::node metierLine = detail["metierLine"];

		sprites.emplace_back(main["backgrnd"]);
		sprites.emplace_back(main["backgrnd2"]);
		sprites.emplace_back(main["backgrnd3"]);

		textures_detail.emplace_back(detail["backgrnd"]);
		textures_detail.emplace_back(detail["backgrnd2"]);
		textures_detail.emplace_back(detail["backgrnd3"]);
		textures_detail.emplace_back(detail["backgrnd4"]);

		abilities[RARE] = abilityTitle["rare"]["0"];
		abilities[EPIC] = abilityTitle["epic"]["0"];
		abilities[UNIQUE] = abilityTitle["unique"]["0"];
		abilities[LEGENDARY] = abilityTitle["legendary"]["0"];
		abilities[NONE] = abilityTitle["normal"]["0"];

		inner_ability[true] = metierLine["activated"]["0"];
		inner_ability[false] = metierLine["disabled"]["0"];

		buttons[BT_CLOSE] = std::make_unique<MapleButton>(close, Point<int16_t>(196, 12));
		buttons[BT_HP] = std::make_unique<MapleButton>(main["BtHpUp"]);
		buttons[BT_MP] = std::make_unique<MapleButton>(main["BtHpUp"], Point<int16_t>(0, 18));		// TODO: "BtMpUp" not Working
		buttons[BT_STR] = std::make_unique<MapleButton>(main["BtHpUp"], Point<int16_t>(0, 87));		// TODO: "BtStrUp" not working
		buttons[BT_DEX] = std::make_unique<MapleButton>(main["BtHpUp"], Point<int16_t>(0, 105));	// TODO: "BtDexUp" not working
		buttons[BT_INT] = std::make_unique<MapleButton>(main["BtHpUp"], Point<int16_t>(0, 123));	// TODO: "BtIntUp" not working
		buttons[BT_LUK] = std::make_unique<MapleButton>(main["BtHpUp"], Point<int16_t>(0, 141));	// TODO: "BtLukUp" not working
		buttons[BT_AUTO] = std::make_unique<MapleButton>(main["BtAuto"]);
		buttons[BT_HYPERSTATOPEN] = std::make_unique<MapleButton>(main["BtHyperStatOpen"]);
		buttons[BT_HYPERSTATCLOSE] = std::make_unique<MapleButton>(main["BtHyperStatClose"]);
		buttons[BT_DETAILOPEN] = std::make_unique<MapleButton>(main["BtDetailOpen"]);
		buttons[BT_DETAILCLOSE] = std::make_unique<MapleButton>(main["BtDetailClose"]);
		buttons[BT_ABILITY] = std::make_unique<MapleButton>(detail["BtAbility"], Point<int16_t>(212, 0));
		buttons[BT_DETAIL_DETAILCLOSE] = std::make_unique<MapleButton>(detail["BtHpUp"], Point<int16_t>(162, 300));

		buttons[BT_HYPERSTATOPEN]->set_active(false);
		buttons[BT_DETAILCLOSE]->set_active(false);
		buttons[BT_ABILITY]->set_active(false);
		buttons[BT_ABILITY]->set_state(Button::State::DISABLED);
		buttons[BT_DETAIL_DETAILCLOSE]->set_active(false);

		update_ap();

		// Normal
		for (size_t i = NAME; i <= LUK; i++)
			statlabels[i] = Text(Text::Font::A11M, Text::Alignment::LEFT, Text::Color::EMPEROR);

		statoffsets[NAME] = Point<int16_t>(73, 26);
		statoffsets[JOB] = Point<int16_t>(74, 44);
		statoffsets[GUILD] = Point<int16_t>(74, 63);
		statoffsets[FAME] = Point<int16_t>(74, 80);
		statoffsets[DAMAGE] = Point<int16_t>(74, 98);
		statoffsets[HP] = Point<int16_t>(74, 116);
		statoffsets[MP] = Point<int16_t>(74, 134);
		statoffsets[AP] = Point<int16_t>(86, 175);
		statoffsets[STR] = Point<int16_t>(73, 204);
		statoffsets[DEX] = Point<int16_t>(73, 222);
		statoffsets[INT] = Point<int16_t>(73, 240);
		statoffsets[LUK] = Point<int16_t>(73, 258);

		// Detailed
		statlabels[DAMAGE_DETAILED] = Text(Text::Font::A11M, Text::Alignment::LEFT, Text::Color::EMPEROR);
		statlabels[DAMAGE_BONUS] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Text::Color::EMPEROR);
		statlabels[BOSS_DAMAGE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Text::Color::EMPEROR);
		statlabels[FINAL_DAMAGE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Text::Color::EMPEROR);
		statlabels[IGNORE_DEFENSE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Text::Color::EMPEROR);
		statlabels[CRITICAL_RATE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Text::Color::EMPEROR);
		statlabels[CRITICAL_DAMAGE] = Text(Text::Font::A11M, Text::Alignment::LEFT, Text::Color::EMPEROR);
		statlabels[STATUS_RESISTANCE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Text::Color::EMPEROR);
		statlabels[KNOCKBACK_RESISTANCE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Text::Color::EMPEROR);
		statlabels[DEFENSE] = Text(Text::Font::A11M, Text::Alignment::LEFT, Text::Color::EMPEROR);
		statlabels[SPEED] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Text::Color::EMPEROR);
		statlabels[JUMP] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Text::Color::EMPEROR);
		statlabels[HONOR] = Text(Text::Font::A11M, Text::Alignment::LEFT, Text::Color::EMPEROR);

		statoffsets[DAMAGE_DETAILED] = Point<int16_t>(73, 38);
		statoffsets[DAMAGE_BONUS] = Point<int16_t>(100, 56);
		statoffsets[BOSS_DAMAGE] = Point<int16_t>(196, 56);
		statoffsets[FINAL_DAMAGE] = Point<int16_t>(100, 74);
		statoffsets[IGNORE_DEFENSE] = Point<int16_t>(196, 74);
		statoffsets[CRITICAL_RATE] = Point<int16_t>(100, 92);
		statoffsets[CRITICAL_DAMAGE] = Point<int16_t>(73, 110);
		statoffsets[STATUS_RESISTANCE] = Point<int16_t>(100, 128);
		statoffsets[KNOCKBACK_RESISTANCE] = Point<int16_t>(196, 128);
		statoffsets[DEFENSE] = Point<int16_t>(73, 146);
		statoffsets[SPEED] = Point<int16_t>(100, 164);
		statoffsets[JUMP] = Point<int16_t>(196, 164);
		statoffsets[HONOR] = Point<int16_t>(73, 283);

		update_all_stats();
		update_stat(Maplestat::JOB);
		update_stat(Maplestat::FAME);

		dimension = Point<int16_t>(212, 318);
		showdetail = false;
	}

	void UIStatsinfo::draw(float alpha) const
	{
		UIElement::draw_sprites(alpha);

		if (showdetail)
		{
			Point<int16_t> detail_pos(position + Point<int16_t>(212, 0));

			textures_detail[0].draw(detail_pos + Point<int16_t>(0, -1));
			textures_detail[1].draw(detail_pos);
			textures_detail[2].draw(detail_pos);
			textures_detail[3].draw(detail_pos);

			abilities[NONE].draw(DrawArgument(detail_pos));

			inner_ability[false].draw(detail_pos);
			inner_ability[false].draw(detail_pos + Point<int16_t>(0, 19));
			inner_ability[false].draw(detail_pos + Point<int16_t>(0, 38));
		}

		size_t last = showdetail ? NUM_LABELS : NUM_NORMAL;

		for (size_t i = 0; i < last; i++)
		{
			Point<int16_t> labelpos = position + statoffsets[i];

			if (i >= NUM_NORMAL)
				labelpos.shift_x(213);

			statlabels[i].draw(labelpos);
		}

		UIElement::draw_buttons(alpha);
	}

	void UIStatsinfo::send_key(int32_t keycode, bool pressed)
	{
		if (keycode == KeyAction::ESCAPE)
			active = false;
	}

	bool UIStatsinfo::is_in_range(Point<int16_t> cursorpos) const
	{
		Point<int16_t> pos_adj;

		if (showdetail)
			pos_adj = Point<int16_t>(211, 25);
		else
			pos_adj = Point<int16_t>(0, 0);

		auto bounds = Rectangle<int16_t>(position, position + dimension + pos_adj);
		return bounds.contains(cursorpos);
	}

	void UIStatsinfo::update_all_stats()
	{
		update_simple(AP, Maplestat::AP);

		if (hasap ^ (stats.get_stat(Maplestat::AP) > 0))
			update_ap();

		statlabels[NAME].change_text(stats.get_name());
		statlabels[GUILD].change_text("-");
		statlabels[HP].change_text(std::to_string(stats.get_stat(Maplestat::HP)) + " / " + std::to_string(stats.get_total(Equipstat::HP)));
		statlabels[MP].change_text(std::to_string(stats.get_stat(Maplestat::MP)) + " / " + std::to_string(stats.get_total(Equipstat::MP)));

		update_basevstotal(STR, Maplestat::STR, Equipstat::STR);
		update_basevstotal(DEX, Maplestat::DEX, Equipstat::DEX);
		update_basevstotal(INT, Maplestat::INT, Equipstat::INT);
		update_basevstotal(LUK, Maplestat::LUK, Equipstat::LUK);

		statlabels[DAMAGE].change_text(std::to_string(stats.get_mindamage()) + " ~ " + std::to_string(stats.get_maxdamage()));

		if (stats.is_damage_buffed())
			statlabels[DAMAGE].change_color(Text::Color::RED);
		else
			statlabels[DAMAGE].change_color(Text::Color::EMPEROR);

		statlabels[DAMAGE_DETAILED].change_text(std::to_string(stats.get_mindamage()) + " ~ " + std::to_string(stats.get_maxdamage()));
		statlabels[DAMAGE_BONUS].change_text("0%");
		statlabels[BOSS_DAMAGE].change_text(std::to_string(static_cast<int32_t>(stats.get_bossdmg() * 100)) + "%");
		statlabels[FINAL_DAMAGE].change_text("0%");
		statlabels[IGNORE_DEFENSE].change_text(std::to_string(static_cast<int32_t>(stats.get_ignoredef())) + "%");
		statlabels[CRITICAL_RATE].change_text(std::to_string(static_cast<int32_t>(stats.get_critical() * 100)) + "%");
		statlabels[CRITICAL_DAMAGE].change_text("0.00%");
		statlabels[STATUS_RESISTANCE].change_text(std::to_string(static_cast<int32_t>(stats.get_resistance())));
		statlabels[KNOCKBACK_RESISTANCE].change_text("0%");
		update_buffed(DEFENSE, Equipstat::WDEF);
		statlabels[SPEED].change_text(std::to_string(stats.get_total(Equipstat::SPEED)) + "%");
		statlabels[JUMP].change_text(std::to_string(stats.get_total(Equipstat::JUMP)) + "%");
		statlabels[HONOR].change_text(std::to_string(stats.get_honor()));
	}

	void UIStatsinfo::update_stat(Maplestat::Id stat)
	{
		switch (stat)
		{
		case Maplestat::JOB:
			statlabels[JOB].change_text(stats.get_jobname());
			break;
		case Maplestat::FAME:
			update_simple(FAME, Maplestat::FAME);
			break;
		}
	}

	Button::State UIStatsinfo::button_pressed(uint16_t id)
	{
		switch (id)
		{
		case BT_CLOSE:
			active = false;
			break;
		case BT_DETAILOPEN:
			showdetail = true;

			buttons[BT_DETAILOPEN]->set_active(false);
			buttons[BT_DETAILCLOSE]->set_active(true);
			buttons[BT_ABILITY]->set_active(true);
			buttons[BT_DETAIL_DETAILCLOSE]->set_active(true);
			break;
		case BT_DETAILCLOSE:
		case BT_DETAIL_DETAILCLOSE:
			showdetail = false;

			buttons[BT_DETAILOPEN]->set_active(true);
			buttons[BT_DETAILCLOSE]->set_active(false);
			buttons[BT_ABILITY]->set_active(false);
			buttons[BT_DETAIL_DETAILCLOSE]->set_active(false);
			break;
		case BT_HP:
			send_apup(Maplestat::HP);
			break;
		case BT_MP:
			send_apup(Maplestat::MP);
			break;
		case BT_STR:
			send_apup(Maplestat::STR);
			break;
		case BT_DEX:
			send_apup(Maplestat::DEX);
			break;
		case BT_INT:
			send_apup(Maplestat::INT);
			break;
		case BT_LUK:
			send_apup(Maplestat::LUK);
			break;
		case BT_HYPERSTATCLOSE:
			int16_t level = 0;

			if (level < 140)
			{
				constexpr char* message = "You can use the Hyper Stat at Lv. 140 and above.";
				auto onok = [&]() {};

				UI::get().emplace<UIOk>(message, onok, UINotice::NoticeType::OK);
			}
		}

		return Button::State::NORMAL;
	}

	void UIStatsinfo::send_apup(Maplestat::Id stat) const
	{
		SpendApPacket(stat).dispatch();
		UI::get().disable();
	}

	void UIStatsinfo::update_ap()
	{
		bool nowap = stats.get_stat(Maplestat::AP) > 0;
		Button::State newstate = nowap ? Button::State::NORMAL : Button::State::DISABLED;

		for (int i = BT_HP; i <= BT_AUTO; i++)
			buttons[i]->set_state(newstate);

		hasap = nowap;
	}

	void UIStatsinfo::update_simple(StatLabel label, Maplestat::Id stat)
	{
		statlabels[label].change_text(std::to_string(stats.get_stat(stat)));
	}

	void UIStatsinfo::update_basevstotal(StatLabel label, Maplestat::Id bstat, Equipstat::Id tstat)
	{
		int32_t base = stats.get_stat(bstat);
		int32_t total = stats.get_total(tstat);
		int32_t delta = total - base;

		std::string stattext = std::to_string(total);

		if (delta)
		{
			stattext += " (" + std::to_string(base);

			if (delta > 0)
				stattext += "+" + std::to_string(delta);
			else if (delta < 0)
				stattext += "-" + std::to_string(-delta);

			stattext += ")";
		}

		statlabels[label].change_text(stattext);
	}

	void UIStatsinfo::update_buffed(StatLabel label, Equipstat::Id stat)
	{
		int32_t total = stats.get_total(stat);
		int32_t delta = stats.get_buffdelta(stat);

		std::string stattext = std::to_string(total);

		if (delta)
		{
			stattext += " (" + std::to_string(total - delta);

			if (delta > 0)
			{
				stattext += "+" + std::to_string(delta);

				statlabels[label].change_color(Text::Color::RED);
			}
			else if (delta < 0)
			{
				stattext += "-" + std::to_string(-delta);

				statlabels[label].change_color(Text::Color::BLUE);
			}

			stattext += ")";
		}

		statlabels[label].change_text(stattext);
	}
}