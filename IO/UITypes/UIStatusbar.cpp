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
#include "UIStatusbar.h"

#include "../UI.h"
#include "../Components/MapleButton.h"

#include "../../Character/ExpTable.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UIStatusbar::UIStatusbar(const CharStats& st)
		: UIElement(POSITION, DIMENSION), chatbar(POSITION), stats(st) {

		nl::node mainbar = nl::nx::ui["StatusBar2.img"]["mainBar"];

		sprites.emplace_back(mainbar["backgrnd"]);
		sprites.emplace_back(mainbar["gaugeBackgrd"]);
		sprites.emplace_back(mainbar["notice"]);
		sprites.emplace_back(mainbar["lvBacktrnd"]);
		sprites.emplace_back(mainbar["lvCover"]);

		expbar = { 
			mainbar.resolve("gauge/exp/0"), 
			mainbar.resolve("gauge/exp/1"),
			mainbar.resolve("gauge/exp/2"), 
			308, 0.0f
		};
		hpbar = {
			mainbar.resolve("gauge/hp/0"),
			mainbar.resolve("gauge/hp/1"),
			mainbar.resolve("gauge/hp/2"),
			137, 0.0f
		};
		mpbar = {
			mainbar.resolve("gauge/mp/0"),
			mainbar.resolve("gauge/mp/1"),
			mainbar.resolve("gauge/mp/2"),
			137, 0.0f
		};

		statset = { mainbar["gauge"]["number"], Charset::RIGHT };
		levelset = { mainbar["lvNumber"], Charset::LEFT };

		joblabel = { Text::A11M, Text::LEFT, Text::YELLOW };
		namelabel = { Text::A13M, Text::LEFT, Text::WHITE };

		buttons[BT_WHISPER] = std::make_unique<MapleButton>(mainbar["BtChat"]);
		buttons[BT_CALLGM] = std::make_unique<MapleButton>(mainbar["BtClaim"]);

		buttons[BT_CASHSHOP] = std::make_unique<MapleButton>(mainbar["BtCashShop"]);
		buttons[BT_TRADE] = std::make_unique<MapleButton>(mainbar["BtMTS"]);
		buttons[BT_MENU] = std::make_unique<MapleButton>(mainbar["BtMenu"]);
		buttons[BT_OPTIONS] = std::make_unique<MapleButton>(mainbar["BtSystem"]);

		buttons[BT_CHARACTER] = std::make_unique<MapleButton>(mainbar["BtCharacter"]);
		buttons[BT_STATS] = std::make_unique<MapleButton>(mainbar["BtStat"]);
		buttons[BT_QUEST] = std::make_unique<MapleButton>(mainbar["BtQuest"]);
		buttons[BT_INVENTORY] = std::make_unique<MapleButton>(mainbar["BtInven"]);
		buttons[BT_EQUIPS] = std::make_unique<MapleButton>(mainbar["BtEquip"]);
		buttons[BT_SKILL] = std::make_unique<MapleButton>(mainbar["BtSkill"]);
	}

	void UIStatusbar::draw(float alpha) const
	{
		UIElement::draw(alpha);

		expbar.draw(position + Point<int16_t>(-261, -15));
		hpbar.draw(position + Point<int16_t>(-261, -31));
		mpbar.draw(position + Point<int16_t>(-90, -31));

		int16_t level = stats.get_stat(Maplestat::LEVEL);
		int16_t hp = stats.get_stat(Maplestat::HP);
		int16_t mp = stats.get_stat(Maplestat::MP);
		int32_t maxhp = stats.get_total(Equipstat::HP);
		int32_t maxmp = stats.get_total(Equipstat::MP);
		int64_t exp = stats.get_exp();

		std::string expstring = std::to_string(100 * getexppercent());
		statset.draw(
			std::to_string(exp) + "[" + expstring.substr(0, expstring.find('.') + 3) + "%]",
			position + Point<int16_t>(47, -13)
			);
		statset.draw(
			"[" + std::to_string(hp) + "/" + std::to_string(maxhp) + "]",
			position + Point<int16_t>(-124, -29)
			);
		statset.draw(
			"[" + std::to_string(mp) + "/" + std::to_string(maxmp) + "]",
			position + Point<int16_t>(47, -29)
			);
		levelset.draw(
			std::to_string(level),
			position + Point<int16_t>(-480, -24)
			);

		joblabel.draw(position + Point<int16_t>(-435, -21));
		namelabel.draw(position + Point<int16_t>(-435, -36));

		chatbar.draw(alpha);
	}

	void UIStatusbar::update()
	{
		UIElement::update();

		chatbar.update();

		expbar.update(getexppercent());
		hpbar.update(gethppercent());
		mpbar.update(getmppercent());

		namelabel.change_text(stats.get_name());
		joblabel.change_text(stats.get_jobname());

		for (auto iter : message_cooldowns)
		{
			iter.second -= Constants::TIMESTEP;
		}
	}

	Button::State UIStatusbar::button_pressed(uint16_t id)
	{
		switch (id)
		{
		case BT_STATS:
			UI::get().send_menu(KeyAction::CHARSTATS);
			return Button::NORMAL;
		case BT_INVENTORY:
			UI::get().send_menu(KeyAction::INVENTORY);
			return Button::NORMAL;
		case BT_EQUIPS:
			UI::get().send_menu(KeyAction::EQUIPS);
			return Button::NORMAL;
		case BT_SKILL:
			UI::get().send_menu(KeyAction::SKILLBOOK);
			return Button::NORMAL;
		}
		return Button::PRESSED;
	}

	bool UIStatusbar::is_in_range(Point<int16_t> cursorpos) const
	{
		Rectangle<int16_t> bounds(
			position - Point<int16_t>(512, 84),
			position - Point<int16_t>(512, 84) + dimension
			);
		return bounds.contains(cursorpos) || chatbar.is_in_range(cursorpos);
	}

	bool UIStatusbar::remove_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (chatbar.remove_cursor(clicked, cursorpos))
			return true;

		return UIElement::remove_cursor(clicked, cursorpos);
	}

	Cursor::State UIStatusbar::send_cursor(bool pressed, Point<int16_t> cursorpos)
	{
		if (chatbar.is_in_range(cursorpos))
		{
			UIElement::send_cursor(pressed, cursorpos);
			return chatbar.send_cursor(pressed, cursorpos);
		}
		else
		{
			chatbar.send_cursor(pressed, cursorpos);
			return UIElement::send_cursor(pressed, cursorpos);
		}
	}

	void UIStatusbar::send_chatline(const std::string& line, UIChatbar::LineType type)
	{
		chatbar.send_line(line, type);
	}

	void UIStatusbar::display_message(Messages::Type line, UIChatbar::LineType type)
	{
		if (message_cooldowns[line] > 0)
			return;

		std::string message{ Messages::messages[line] };
	    chatbar.send_line(message, type);

		message_cooldowns[line] = MESSAGE_COOLDOWN;
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