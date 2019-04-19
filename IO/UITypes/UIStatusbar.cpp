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

#include "../Components/MapleButton.h"

#include "../../Character/ExpTable.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UIStatusbar::UIStatusbar(const CharStats& st) : stats(st)
	{
		nl::node mainBar = nl::nx::ui["StatusBar3.img"]["mainBar"];
		nl::node status = mainBar["status800"];
		nl::node EXPBar = mainBar["EXPBar"];
		nl::node menu = mainBar["menu"];
		nl::node quickSlot = mainBar["quickSlot"];

		exp_pos = Point<int16_t>(0, 87);

		sprites.emplace_back(EXPBar["backgrnd"], DrawArgument(Point<int16_t>(0, 86), Point<int16_t>(800, 0)));
		sprites.emplace_back(EXPBar["800"]["layer:back"], exp_pos);

		expbar = Gauge(
			EXPBar.resolve("800/layer:gauge"),
			EXPBar.resolve("800/layer:cover"),
			EXPBar.resolve("layer:effect"),
			308, 0.0f
		);

		hpmp_pos = Point<int16_t>(412, 40);

		hpmp_sprites.emplace_back(status["backgrnd"], hpmp_pos - Point<int16_t>(1, 0));
		hpmp_sprites.emplace_back(status["layer:cover"], hpmp_pos - Point<int16_t>(1, 0));
		hpmp_sprites.emplace_back(status["layer:Lv"], hpmp_pos);

		hpbar = Gauge(status.resolve("gauge/hp/layer:0"), 139, 0.0f);
		mpbar = Gauge(status.resolve("gauge/mp/layer:0"), 139, 0.0f);

		statset = Charset(EXPBar["number"], Charset::Alignment::RIGHT);
		hpmpset = Charset(status["gauge"]["number"], Charset::Alignment::RIGHT);
		levelset = Charset(status["lvNumber"], Charset::Alignment::LEFT);

		namelabel = OutlinedText(Text::Font::A13M, Text::Alignment::LEFT, Text::Color::SILVER, Text::Color::SLATE);

		quickslot_pos = Point<int16_t>(579, 0);

		quickslot[0] = quickSlot["backgrnd"];
		quickslot[1] = quickSlot["layer:cover"];

		auto buttonPos = Point<int16_t>(591, 73);

		buttons[BT_CASHSHOP] = std::make_unique<MapleButton>(menu["button:CashShop"], buttonPos);
		buttons[BT_MENU] = std::make_unique<MapleButton>(menu["button:Menu"], buttonPos);
		buttons[BT_OPTIONS] = std::make_unique<MapleButton>(menu["button:Setting"], buttonPos);
		buttons[BT_CHARACTER] = std::make_unique<MapleButton>(menu["button:Character"], buttonPos);
		buttons[BT_COMMUNITY] = std::make_unique<MapleButton>(menu["button:Community"], buttonPos);
		buttons[BT_EVENT] = std::make_unique<MapleButton>(menu["button:Event"], buttonPos);

		buttons[BT_FOLD_QS] = std::make_unique<MapleButton>(quickSlot["button:Fold800"], Point<int16_t>(579, 0));
		buttons[BT_EXTEND_QS] = std::make_unique<MapleButton>(quickSlot["button:Extend800"], Point<int16_t>(791, 0));
		buttons[BT_EXTEND_QS]->set_active(false);

		quickslot_active = true;
		position = Point<int16_t>(0, 470);
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
			position + Point<int16_t>(427, 111)
		);

		hpmpset.draw(
			"[" + std::to_string(hp) + "/" + std::to_string(maxhp) + "]",
			position + Point<int16_t>(530, 70)
		);

		hpmpset.draw(
			"[" + std::to_string(mp) + "/" + std::to_string(maxmp) + "]",
			position + Point<int16_t>(528, 86)
		);

		levelset.draw(
			std::to_string(level),
			position + Point<int16_t>(461, 48)
		);

		namelabel.draw(position + Point<int16_t>(487, 40));

		if (quickslot_active)
		{
			quickslot[0].draw(position + quickslot_pos);
			quickslot[1].draw(position + quickslot_pos);
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
			quickslot_active = false;
			buttons[BT_FOLD_QS]->set_active(false);
			buttons[BT_EXTEND_QS]->set_active(true);
			return Button::NORMAL;
		case BT_EXTEND_QS:
			quickslot_active = true;
			buttons[BT_FOLD_QS]->set_active(true);
			buttons[BT_EXTEND_QS]->set_active(false);
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
		quickslot_active = !quickslot_active;
		buttons[BT_FOLD_QS]->set_active(quickslot_active);
		buttons[BT_EXTEND_QS]->set_active(!quickslot_active);
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