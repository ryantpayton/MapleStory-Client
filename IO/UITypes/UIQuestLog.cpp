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
#include "UIQuestLog.h"

#include "../IO/Components/MapleButton.h"
#include "../IO/Components/TwoSpriteButton.h"
#include "../IO/KeyAction.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UIQuestLog::UIQuestLog(const Questlog& ql) : UIDragElement<PosQUEST>(Point<int16_t>(172, 20)), questlog(ql)
	{
		tab = 0;

		nl::node close = nl::nx::ui["Basic.img"]["BtClose"];
		nl::node quest = nl::nx::ui["UIWindow2.img"]["Quest"];
		nl::node list = quest["list"];

		sprites.emplace_back(list["backgrnd"]);
		sprites.emplace_back(list["backgrnd2"]);
		sprites.emplace_back(list["searchArea"]);
		sprites_notice.emplace_back(list["notice0"]);
		sprites_notice.emplace_back(list["notice1"]);
		sprites_notice.emplace_back(list["notice2"]);

		nl::node taben = list["Tab"]["enabled"];
		nl::node tabdis = list["Tab"]["disabled"];

		buttons[BT_CLOSE] = std::make_unique<MapleButton>(close, Point<int16_t>(276, 12));
		buttons[BT_SEARCH] = std::make_unique<MapleButton>(list["BtSearch"], Point<int16_t>(256, 12));

		buttons[BT_TAB_AVAILABLE] = std::make_unique<TwoSpriteButton>(tabdis["0"], taben["0"]);
		buttons[BT_TAB_IN_PROGRESS] = std::make_unique<TwoSpriteButton>(tabdis["1"], taben["1"]);
		buttons[BT_TAB_COMPLETED] = std::make_unique<TwoSpriteButton>(tabdis["2"], taben["2"]);

		buttons[BT_ALL_LEVEL] = std::make_unique<MapleButton>(quest["BtAllLevel"]);
		buttons[BT_ALL_LOCATION] = std::make_unique<MapleButton>(quest["BtAllLocation"]);

		change_tab(0);

		dimension = { 276, 335 };
		active = true;
	}

	void UIQuestLog::draw(float alpha) const
	{
		UIElement::draw(alpha);

		sprites_notice[tab].draw(position, alpha);
	}

	void UIQuestLog::update()
	{
		UIElement::update();
	}

	void UIQuestLog::send_key(int32_t keycode, bool pressed)
	{
		if (pressed)
		{
			if (keycode == KeyAction::ESCAPE)
			{
				active = false;
			}
			else if (keycode == KeyAction::TAB)
			{
				uint16_t new_tab = tab;

				if (new_tab < BT_TAB_COMPLETED)
					new_tab++;
				else
					new_tab = BT_TAB_AVAILABLE;

				change_tab(new_tab);
			}
		}
	}

	Button::State UIQuestLog::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
		case BT_CLOSE:
			active = false;
			break;
		case BT_TAB_AVAILABLE:
		case BT_TAB_IN_PROGRESS:
		case BT_TAB_COMPLETED:
			change_tab(buttonid);
			break;
		}

		return Button::State::NORMAL;
	}

	void UIQuestLog::change_tab(uint8_t tabid)
	{
		uint8_t oldtab = tab;
		tab = tabid;

		if (oldtab != tab)
			buttons[BT_TAB_AVAILABLE + oldtab]->set_state(Button::State::NORMAL);

		buttons[BT_TAB_AVAILABLE + tab]->set_state(Button::State::PRESSED);
	}
}