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
#include "UIStatusbar.h"

#include "../Configuration.h"
#include "../Constants.h"

#include "../Components/MapleButton.h"

#include "../../Character/ExpTable.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UIStatusbar::UIStatusbar(const CharStats& st) : stats(st)
	{
		quickslot_active = true;
		VWIDTH = Constants::Constants::get().get_viewwidth();

		std::string stat = "status";

		if (VWIDTH == 800)
			stat += "800";

		nl::node mainBar = nl::nx::ui["StatusBar3.img"]["mainBar"];
		nl::node status = mainBar[stat];
		nl::node EXPBar = mainBar["EXPBar"];
		nl::node EXPBarRes = EXPBar[VWIDTH];
		nl::node menu = mainBar["menu"];
		nl::node quickSlot = mainBar["quickSlot"];

		exp_pos = Point<int16_t>(0, 87);

		sprites.emplace_back(EXPBar["backgrnd"], DrawArgument(Point<int16_t>(0, 87), Point<int16_t>(VWIDTH, 0)));
		sprites.emplace_back(EXPBarRes["layer:back"], exp_pos);

		int16_t exp_max = VWIDTH - 15;

		expbar = Gauge(
			EXPBarRes.resolve("layer:gauge"),
			EXPBarRes.resolve("layer:cover"),
			EXPBar.resolve("layer:effect"),
			exp_max, 0.0f
		);

		int16_t pos_adj = 0;

		if (VWIDTH == 1280)
			pos_adj = 87;
		else if (VWIDTH == 1366)
			pos_adj = 171;
		else if (VWIDTH == 1920)
			pos_adj = 448;

		if (VWIDTH == 800)
		{
			hpmp_pos = Point<int16_t>(412, 40);
			hpset_pos = Point<int16_t>(530, 70);
			mpset_pos = Point<int16_t>(528, 86);
			statset_pos = Point<int16_t>(427, 111);
			levelset_pos = Point<int16_t>(461, 48);
			namelabel_pos = Point<int16_t>(487, 40);
			quickslot_pos = Point<int16_t>(579, 0);
		}
		else
		{
			hpmp_pos = Point<int16_t>(416 + pos_adj, 40);
			hpset_pos = Point<int16_t>(550 + pos_adj, 70);
			mpset_pos = Point<int16_t>(546 + pos_adj, 86);
			statset_pos = Point<int16_t>(539 + pos_adj, 111);
			levelset_pos = Point<int16_t>(465 + pos_adj, 48);
			namelabel_pos = Point<int16_t>(493 + pos_adj, 40);
			quickslot_pos = Point<int16_t>(628 + pos_adj, 37);
		}

		if (VWIDTH == 1280)
		{
			statset_pos = Point<int16_t>(580 + pos_adj, 111);
			quickslot_pos = Point<int16_t>(622 + pos_adj, 37);
		}
		else if (VWIDTH == 1366)
		{
			quickslot_pos = Point<int16_t>(623 + pos_adj, 37);
		}
		else if (VWIDTH == 1920)
		{
			quickslot_pos = Point<int16_t>(900 + pos_adj, 37);
		}

		hpmp_sprites.emplace_back(status["backgrnd"], hpmp_pos - Point<int16_t>(1, 0));
		hpmp_sprites.emplace_back(status["layer:cover"], hpmp_pos - Point<int16_t>(1, 0));

		if (VWIDTH == 800)
			hpmp_sprites.emplace_back(status["layer:Lv"], hpmp_pos);
		else
			hpmp_sprites.emplace_back(status["layer:Lv"], hpmp_pos - Point<int16_t>(1, 0));

		int16_t hpmp_max = 139;

		if (VWIDTH > 800)
			hpmp_max += 30;

		hpbar = Gauge(status.resolve("gauge/hp/layer:0"), hpmp_max, 0.0f);
		mpbar = Gauge(status.resolve("gauge/mp/layer:0"), hpmp_max, 0.0f);

		statset = Charset(EXPBar["number"], Charset::Alignment::RIGHT);
		hpmpset = Charset(status["gauge"]["number"], Charset::Alignment::RIGHT);
		levelset = Charset(status["lvNumber"], Charset::Alignment::LEFT);

		namelabel = OutlinedText(Text::Font::A13M, Text::Alignment::LEFT, Text::Color::SILVER, Text::Color::SLATE);

		quickslot[0] = quickSlot["backgrnd"];
		quickslot[1] = quickSlot["layer:cover"];

		Point<int16_t> buttonPos = Point<int16_t>(591 + pos_adj, 73);

		if (VWIDTH == 1024)
			buttonPos += Point<int16_t>(38, 0);
		else if (VWIDTH == 1280)
			buttonPos += Point<int16_t>(31, 0);
		else if (VWIDTH == 1366)
			buttonPos += Point<int16_t>(33, 0);
		else if (VWIDTH == 1920)
			buttonPos += Point<int16_t>(310, 0);

		buttons[BT_CASHSHOP] = std::make_unique<MapleButton>(menu["button:CashShop"], buttonPos);
		buttons[BT_MENU] = std::make_unique<MapleButton>(menu["button:Menu"], buttonPos);
		buttons[BT_OPTIONS] = std::make_unique<MapleButton>(menu["button:Setting"], buttonPos);
		buttons[BT_CHARACTER] = std::make_unique<MapleButton>(menu["button:Character"], buttonPos);
		buttons[BT_COMMUNITY] = std::make_unique<MapleButton>(menu["button:Community"], buttonPos);
		buttons[BT_EVENT] = std::make_unique<MapleButton>(menu["button:Event"], buttonPos);

		if (quickslot_active && VWIDTH > 800)
		{
			buttons[BT_CASHSHOP]->set_active(false);
			buttons[BT_MENU]->set_active(false);
			buttons[BT_OPTIONS]->set_active(false);
			buttons[BT_CHARACTER]->set_active(false);
			buttons[BT_COMMUNITY]->set_active(false);
			buttons[BT_EVENT]->set_active(false);
		}

		std::string fold = "button:Fold";
		std::string extend = "button:Extend";

		if (VWIDTH == 800)
		{
			fold += "800";
			extend += "800";
		}

		if (VWIDTH == 800)
		{
			buttons[BT_FOLD_QS] = std::make_unique<MapleButton>(quickSlot[fold], Point<int16_t>(579, 0));
			buttons[BT_EXTEND_QS] = std::make_unique<MapleButton>(quickSlot[extend], Point<int16_t>(791, 0));
		}
		else if (VWIDTH == 1024)
		{
			buttons[BT_FOLD_QS] = std::make_unique<MapleButton>(quickSlot[fold], Point<int16_t>(628 + pos_adj, 37));
			buttons[BT_EXTEND_QS] = std::make_unique<MapleButton>(quickSlot[extend], Point<int16_t>(838 + pos_adj, 37));
		}
		else if (VWIDTH == 1280)
		{
			buttons[BT_FOLD_QS] = std::make_unique<MapleButton>(quickSlot[fold], Point<int16_t>(621 + pos_adj, 37));
			buttons[BT_EXTEND_QS] = std::make_unique<MapleButton>(quickSlot[extend], Point<int16_t>(832 + pos_adj, 37));
		}
		else if (VWIDTH == 1366)
		{
			buttons[BT_FOLD_QS] = std::make_unique<MapleButton>(quickSlot[fold], Point<int16_t>(623 + pos_adj, 37));
			buttons[BT_EXTEND_QS] = std::make_unique<MapleButton>(quickSlot[extend], Point<int16_t>(834 + pos_adj, 37));
		}
		else if (VWIDTH == 1920)
		{
			buttons[BT_FOLD_QS] = std::make_unique<MapleButton>(quickSlot[fold], Point<int16_t>(900 + pos_adj, 37));
			buttons[BT_EXTEND_QS] = std::make_unique<MapleButton>(quickSlot[extend], Point<int16_t>(1111 + pos_adj, 37));
		}

		if (quickslot_active)
			buttons[BT_EXTEND_QS]->set_active(false);
		else
			buttons[BT_FOLD_QS]->set_active(false);

		if (VWIDTH == 800)
			position = Point<int16_t>(0, 470);
		else if (VWIDTH == 1024)
			position = Point<int16_t>(0, 638);
		else if (VWIDTH == 1280)
			position = Point<int16_t>(0, 590);
		else if (VWIDTH == 1366)
			position = Point<int16_t>(0, 638);
		else if (VWIDTH == 1920)
			position = Point<int16_t>(0, 950);

		dimension = Point<int16_t>(390, 130);
	}

	void UIStatusbar::draw(float alpha) const
	{
		UIElement::draw(alpha);

		hpmp_sprites[0].draw(position, alpha);

		expbar.draw(position + exp_pos);
		hpbar.draw(position + hpmp_pos);
		mpbar.draw(position + hpmp_pos);

		hpmp_sprites[1].draw(position, alpha);
		hpmp_sprites[2].draw(position, alpha);

		int16_t level = stats.get_stat(Maplestat::LEVEL);
		int16_t hp = stats.get_stat(Maplestat::HP);
		int16_t mp = stats.get_stat(Maplestat::MP);
		int32_t maxhp = stats.get_total(Equipstat::HP);
		int32_t maxmp = stats.get_total(Equipstat::MP);
		int64_t exp = stats.get_exp();

		std::string expstring = std::to_string(100 * getexppercent());

		statset.draw(
			std::to_string(exp) + "[" + expstring.substr(0, expstring.find('.') + 3) + "%]",
			position + statset_pos
		);

		hpmpset.draw(
			"[" + std::to_string(hp) + "/" + std::to_string(maxhp) + "]",
			position + hpset_pos
		);

		hpmpset.draw(
			"[" + std::to_string(mp) + "/" + std::to_string(maxmp) + "]",
			position + mpset_pos
		);

		levelset.draw(
			std::to_string(level),
			position + levelset_pos
		);

		namelabel.draw(position + namelabel_pos);

		if (quickslot_active)
		{
			quickslot[0].draw(position + quickslot_pos);
			quickslot[1].draw(position + quickslot_pos);
		}
		else
		{
			if (VWIDTH > 800 && VWIDTH < 1366)
			{
				quickslot[0].draw(position + quickslot_pos + Point<int16_t>(210, 0));
				quickslot[1].draw(position + quickslot_pos + Point<int16_t>(210, 0));
			}
			else
			{
				quickslot[0].draw(position + quickslot_pos + Point<int16_t>(211, 0));
				quickslot[1].draw(position + quickslot_pos + Point<int16_t>(211, 0));
			}
		}
	}

	void UIStatusbar::update()
	{
		UIElement::update();

		for each (auto sprite in hpmp_sprites)
			sprite.update();

		expbar.update(getexppercent());
		hpbar.update(gethppercent());
		mpbar.update(getmppercent());

		namelabel.change_text(stats.get_name());
	}

	Button::State UIStatusbar::button_pressed(uint16_t id)
	{
		switch (id)
		{
		case BT_FOLD_QS:
			toggle_qs(false);
			return Button::NORMAL;
		case BT_EXTEND_QS:
			toggle_qs(true);
			return Button::NORMAL;
		}

		return Button::NORMAL;
	}

	bool UIStatusbar::is_in_range(Point<int16_t> cursorpos) const
	{
		auto pos = Point<int16_t>(410, position.y());
		auto bounds = Rectangle<int16_t>(pos, pos + dimension);

		return bounds.contains(cursorpos);
	}

	void UIStatusbar::toggle_qs()
	{
		toggle_qs(!quickslot_active);
	}

	void UIStatusbar::toggle_qs(bool quick_slot_active)
	{
		if (quickslot_active == quick_slot_active)
			return;

		quickslot_active = quick_slot_active;
		buttons[BT_FOLD_QS]->set_active(quickslot_active);
		buttons[BT_EXTEND_QS]->set_active(!quickslot_active);

		if (VWIDTH > 800)
		{
			buttons[BT_CASHSHOP]->set_active(!quickslot_active);
			buttons[BT_MENU]->set_active(!quickslot_active);
			buttons[BT_OPTIONS]->set_active(!quickslot_active);
			buttons[BT_CHARACTER]->set_active(!quickslot_active);
			buttons[BT_COMMUNITY]->set_active(!quickslot_active);
			buttons[BT_EVENT]->set_active(!quickslot_active);
		}
	}

	float UIStatusbar::getexppercent() const
	{
		int16_t level = stats.get_stat(Maplestat::LEVEL);

		if (level >= ExpTable::LEVELCAP)
			return 0.0f;

		int64_t exp = stats.get_exp();

		return static_cast<float>(
			static_cast<double>(exp) / ExpTable::values[level]
			);
	}

	float UIStatusbar::gethppercent() const
	{
		int16_t hp = stats.get_stat(Maplestat::HP);
		int32_t maxhp = stats.get_total(Equipstat::HP);

		return static_cast<float>(hp) / maxhp;
	}

	float UIStatusbar::getmppercent() const
	{
		int16_t mp = stats.get_stat(Maplestat::MP);
		int32_t maxmp = stats.get_total(Equipstat::MP);

		return static_cast<float>(mp) / maxmp;
	}
}