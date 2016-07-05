/////////////////////////////////////////////////////////////////////////////
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

#include "../../Net/Packets/PlayerPackets.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UIStatsinfo::UIStatsinfo(const CharStats& st) :
		UIDragElement<PosSTATS>(Point<int16_t>(212, 20)), stats(st) {

		nl::node src = nl::nx::ui["UIWindow4.img"]["Stat"]["main"];
		nl::node detail = nl::nx::ui["UIWindow4.img"]["Stat"]["detail"];

		sprites.emplace_back(src["backgrnd"]);
		sprites.emplace_back(src["backgrnd2"]);
		sprites.emplace_back(src["backgrnd3"]);

		textures_detail.emplace_back(detail["backgrnd"]);
		textures_detail.emplace_back(detail["backgrnd2"]);
		textures_detail.emplace_back(detail["backgrnd3"]);

		abilities[RARE] = detail["abilityTitle"]["rare"]["0"];
		abilities[EPIC] = detail["abilityTitle"]["epic"]["0"];
		abilities[UNIQUE] = detail["abilityTitle"]["unique"]["0"];
		abilities[LEGENDARY] = detail["abilityTitle"]["legendary"]["0"];
		abilities[NONE] = detail["abilityTitle"]["normal"]["0"];

		buttons[BT_HP] = std::make_unique<MapleButton>(src["BtHpUp"]);
		buttons[BT_MP] = std::make_unique<MapleButton>(src["BtMpUp"]);
		buttons[BT_STR] = std::make_unique<MapleButton>(src["BtStrUp"]);
		buttons[BT_DEX] = std::make_unique<MapleButton>(src["BtDexUp"]);
		buttons[BT_LUK] = std::make_unique<MapleButton>(src["BtLukUp"]);
		buttons[BT_INT] = std::make_unique<MapleButton>(src["BtIntUp"]);

		buttons[BT_DETAILOPEN] = std::make_unique<MapleButton>(src["BtDetailOpen"]);
		buttons[BT_DETAILCLOSE] = std::make_unique<MapleButton>(src["BtDetailClose"]);
		buttons[BT_DETAILCLOSE]->set_active(false);

		update_ap();

		for (size_t i = 0; i < NUMLABELS; i++)
		{
			statlabels[i] = Text(Text::A11M, Text::LEFT, Text::LIGHTGREY);
		}
		statoffsets[NAME] = Point<int16_t>(73, 27);
		statoffsets[JOB] = Point<int16_t>(73, 45);
		statoffsets[GUILD] = Point<int16_t>(73, 63);
		statoffsets[FAME] = Point<int16_t>(73, 81);
		statoffsets[DAMAGE] = Point<int16_t>(73, 99);
		statoffsets[HP] = Point<int16_t>(73, 117);
		statoffsets[MP] = Point<int16_t>(73, 135);
		statoffsets[AP] = Point<int16_t>(70, 177);
		statoffsets[STR] = Point<int16_t>(73, 204);
		statoffsets[DEX] = Point<int16_t>(73, 222);
		statoffsets[INT] = Point<int16_t>(73, 240);
		statoffsets[LUK] = Point<int16_t>(73, 258);
		statoffsets[ATTACK] = Point<int16_t>(73, 37);
		statoffsets[CRIT] = Point<int16_t>(73, 55);
		statoffsets[MINCRIT] = Point<int16_t>(73, 73);
		statoffsets[MAXCRIT] = Point<int16_t>(168, 73);
		statoffsets[BDM] = Point<int16_t>(73, 91);
		statoffsets[IGNOREDEF] = Point<int16_t>(168, 91);
		statoffsets[RESIST] = Point<int16_t>(73, 109);
		statoffsets[STANCE] = Point<int16_t>(168, 109);
		statoffsets[WDEF] = Point<int16_t>(73, 127);
		statoffsets[MDEF] = Point<int16_t>(73, 145);
		statoffsets[ACCURACY] = Point<int16_t>(73, 163);
		statoffsets[AVOID] = Point<int16_t>(73, 199);
		statoffsets[SPEED] = Point<int16_t>(73, 235);
		statoffsets[JUMP] = Point<int16_t>(168, 235);
		statoffsets[HONOR] = Point<int16_t>(73, 353);

		update_all_stats();
		update_stat(Maplestat::JOB);
		update_stat(Maplestat::FAME);

		dimension = Point<int16_t>(212, 318);
		showdetail = false;
	}

	void UIStatsinfo::draw(float alpha) const
	{
		UIElement::draw(alpha);

		if (showdetail)
		{
			Point<int16_t> detail_pos(position + Point<int16_t>(213, 0));
			for (auto& texture : textures_detail)
			{
				texture.draw(detail_pos);
			}
			abilities[NONE].draw({ detail_pos });
		}

		size_t last = showdetail ? NUMLABELS : NUMNORMAL;
		for (size_t i = 0; i < last; i++)
		{
			Point<int16_t> labelpos = position + statoffsets[i];
			if (i >= NUMNORMAL)
				labelpos.shift_x(213);

			statlabels[i].draw(labelpos);
		}
	}

	void UIStatsinfo::update_all_stats()
	{
		update_simple(AP, Maplestat::AP);
		if (hasap ^ (stats.get_stat(Maplestat::AP) > 0))
		{
			update_ap();
		}

		statlabels[NAME].change_text(stats.get_name());
		statlabels[GUILD].change_text("");

		statlabels[HP].change_text(std::to_string(stats.get_stat(Maplestat::HP)) + " / " + std::to_string(stats.get_total(Equipstat::HP)));
		statlabels[MP].change_text(std::to_string(stats.get_stat(Maplestat::MP)) + " / " + std::to_string(stats.get_total(Equipstat::MP)));

		update_basevstotal(STR, Maplestat::STR, Equipstat::STR);
		update_basevstotal(DEX, Maplestat::DEX, Equipstat::DEX);
		update_basevstotal(INT, Maplestat::INT, Equipstat::INT);
		update_basevstotal(LUK, Maplestat::LUK, Equipstat::LUK);

		statlabels[DAMAGE].change_text(std::to_string(stats.get_mindamage()) + " ~ " + std::to_string(stats.get_maxdamage()));
		if (stats.is_damage_buffed())
		{
			statlabels[DAMAGE].change_color(Text::RED);
		}
		else
		{
			statlabels[DAMAGE].change_color(Text::LIGHTGREY);
		}

		update_buffed(ATTACK, Equipstat::WATK);
		update_buffed(WDEF, Equipstat::WDEF);
		update_buffed(MDEF, Equipstat::MDEF);
		update_buffed(ACCURACY, Equipstat::ACC);
		update_buffed(AVOID, Equipstat::AVOID);

		statlabels[CRIT].change_text(std::to_string(static_cast<int32_t>(stats.get_critical() * 100)) + "%");
		statlabels[MINCRIT].change_text(std::to_string(static_cast<int32_t>(stats.get_mincrit() * 100)) + "%");
		statlabels[MAXCRIT].change_text(std::to_string(static_cast<int32_t>(stats.get_maxcrit() * 100)) + "%");
		statlabels[BDM].change_text(std::to_string(static_cast<int32_t>(stats.get_bossdmg() * 100)) + "%");
		statlabels[IGNOREDEF].change_text(std::to_string(static_cast<int32_t>(stats.get_ignoredef() * 100)) + "%");
		statlabels[RESIST].change_text(std::to_string(static_cast<int32_t>(stats.get_resistance() * 100)) + "%");
		statlabels[STANCE].change_text(std::to_string(static_cast<int32_t>(stats.get_stance() * 100)) + "%");
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
		case BT_DETAILOPEN:
			showdetail = true;
			buttons[BT_DETAILOPEN]->set_active(false);
			buttons[BT_DETAILCLOSE]->set_active(true);
			break;
		case BT_DETAILCLOSE:
			showdetail = false;
			buttons[BT_DETAILCLOSE]->set_active(false);
			buttons[BT_DETAILOPEN]->set_active(true);
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
		}
		return Button::NORMAL;
	}

	void UIStatsinfo::send_apup(Maplestat::Id stat) const
	{
		SpendApPacket(stat).dispatch();
		UI::get().disable();
	}

	void UIStatsinfo::update_ap()
	{
		Button::State newstate;
		bool nowap = stats.get_stat(Maplestat::AP) > 0;
		if (nowap)
		{
			newstate = Button::NORMAL;

			buttons[BT_HP]->set_position(Point<int16_t>(20, -36));
			buttons[BT_MP]->set_position(Point<int16_t>(20, -18));
			buttons[BT_STR]->set_position(Point<int16_t>(20, 51));
			buttons[BT_DEX]->set_position(Point<int16_t>(20, 69));
			buttons[BT_INT]->set_position(Point<int16_t>(20, 87));
			buttons[BT_LUK]->set_position(Point<int16_t>(20, 105));
		}
		else
		{
			newstate = Button::DISABLED;

			buttons[BT_HP]->set_position(Point<int16_t>(-48, 14));
			buttons[BT_MP]->set_position(Point<int16_t>(-48, 32));
			buttons[BT_STR]->set_position(Point<int16_t>(-48, 101));
			buttons[BT_DEX]->set_position(Point<int16_t>(-48, 119));
			buttons[BT_INT]->set_position(Point<int16_t>(-48, 137));
			buttons[BT_LUK]->set_position(Point<int16_t>(-48, 155));
		}
		buttons[BT_HP]->set_state(newstate);
		buttons[BT_MP]->set_state(newstate);
		buttons[BT_STR]->set_state(newstate);
		buttons[BT_DEX]->set_state(newstate);
		buttons[BT_LUK]->set_state(newstate);
		buttons[BT_INT]->set_state(newstate);

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
			{
				stattext += " + " + std::to_string(delta);
			}
			else if (delta < 0)
			{
				stattext += " - " + std::to_string(-delta);
			}
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
				stattext += " + " + std::to_string(delta);

				statlabels[label].change_color(Text::RED);
			}
			else if (delta < 0)
			{
				stattext += " - " + std::to_string(-delta);

				statlabels[label].change_color(Text::BLUE);
			}
			stattext += ")";
		}
		statlabels[label].change_text(stattext);
	}
}