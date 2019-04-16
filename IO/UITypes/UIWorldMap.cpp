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
#include "UIWorldMap.h"

#include "../IO/Components/MapleButton.h"

#include "../Template/Rectangle.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UIWorldMap::UIWorldMap() : UIDragElement<PosMAP>(Point<int16_t>(648, 20))
	{
		nl::node close = nl::nx::ui["Basic.img"]["BtClose"];
		nl::node WorldMap = nl::nx::ui["UIWindow2.img"]["WorldMap"];
		nl::node WorldMapSearch = WorldMap["WorldMapSearch"];

		sprites.emplace_back(WorldMap["Border"]["0"]);

		search_background = WorldMapSearch["backgrnd"];
		search_notice = WorldMapSearch["notice"];

		buttons[BT_CLOSE] = std::make_unique<MapleButton>(close, Point<int16_t>(638, 10));
		buttons[BT_SEARCH] = std::make_unique<MapleButton>(WorldMap["BtSearch"]);

		buttons[BT_AUTOFLY] = std::make_unique<MapleButton>(WorldMap["BtAutoFly_1"]);
		buttons[BT_NAVIREG] = std::make_unique<MapleButton>(WorldMap["BtNaviRegister"]);

		buttons[BT_SEARCH_CLOSE] = std::make_unique<MapleButton>(close, Point<int16_t>(785, 10));
		buttons[BT_ALLSEARCH] = std::make_unique<MapleButton>(WorldMapSearch["BtAllsearch"], Point<int16_t>(654, 0));

		buttons[BT_SEARCH_CLOSE]->set_active(false);
		buttons[BT_ALLSEARCH]->set_active(false);

		size_t text_limit = 15;
		search_text = Textfield(Text::Font::A11M, Text::Alignment::LEFT, Text::Color::BLACK, Rectangle<int16_t>(Point<int16_t>(670, 20), Point<int16_t>(745, 40)), text_limit);
		search_text.set_state(Textfield::State::DISABLED);

		search = false;
		dimension = { 648, 535 };
		active = true;
	}

	void UIWorldMap::draw(float alpha) const
	{
		UIElement::draw_sprites(alpha);

		if (search)
		{
			search_background.draw(position + Point<int16_t>(654, 0), alpha);
			search_notice.draw(position + Point<int16_t>(654, 0), alpha);
			search_text.draw(position);
		}

		UIElement::draw_buttons(alpha);
	}

	void UIWorldMap::update()
	{
		UIElement::update();

		if (search)
		{
			search_background.update();
			search_notice.update();
			search_text.update(position);
		}
	}

	void UIWorldMap::send_key(int32_t keycode, bool pressed)
	{
		if (keycode == KeyAction::ESCAPE)
			active = false;
	}

	Button::State UIWorldMap::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
		case BT_CLOSE:
			active = false;
			break;
		case BT_SEARCH:
			if (!search)
			{
				search = true;
				buttons[BT_SEARCH_CLOSE]->set_active(true);
				buttons[BT_ALLSEARCH]->set_active(true);
				search_text.set_state(Textfield::State::NORMAL);
				dimension = { 795, 535 };
				dragarea = { 795, 20 };
			}
			else
			{
				close_search();
			}

			break;
		case BT_SEARCH_CLOSE:
			close_search();
			break;
		}

		return Button::State::NORMAL;
	}

	Cursor::State UIWorldMap::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (Cursor::State new_state = search_text.send_cursor(cursorpos, clicked))
			return new_state;

		return UIDragElement::send_cursor(clicked, cursorpos);
	}

	void UIWorldMap::close_search()
	{
		search = false;
		buttons[BT_SEARCH_CLOSE]->set_active(false);
		buttons[BT_ALLSEARCH]->set_active(false);
		search_text.set_state(Textfield::State::DISABLED);
		dimension = { 648, 535 };
		dragarea = { 648, 20 };
	}
}