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
#include "UICharCreation.h"
#include "UIClassCreation.h"

#include "../Components/MapleButton.h"
#include "../UI.h"
#include "../UITypes/UILoginNotice.h"
#include "../UITypes/UICharSelect.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UICharCreation::UICharCreation()
	{
		version = { Text::Font::A11M, Text::Alignment::LEFT, Text::Color::DARKSTEEL, "Ver. 203.3" };

		nl::node login = nl::nx::ui["Login.img"];
		raceSelect = login["RaceSelect_new"];

		sprites.emplace_back(raceSelect["Back"]["1"]["0"], Point<int16_t>(0, -10));
		sprites.emplace_back(login["Common"]["frame"], Point<int16_t>(399, 289));

		classMap[BT_CLASS_SEL0] = Classes::ARAN;
		classMap[BT_CLASS_SEL1] = Classes::EVAN;
		classMap[BT_CLASS_SEL2] = Classes::CYGNUS;
		classMap[BT_CLASS_SEL3] = Classes::EXPLORERS;
		classMap[BT_CLASS_SEL4] = Classes::DUAL_BLADE;

		for (int i = 0; i < classMap.size(); i++)
		{
			std::string buttonType = "button";
			int8_t buttonIndex = BT_CLASS_SEL0 + i;
			int8_t classIndex = classMap.at(buttonIndex);

			if (classIndex == Classes::EVAN || classIndex == Classes::DUAL_BLADE)
				buttonType = "buttonDisabled";

			buttons[buttonIndex] = std::make_unique<MapleButton>(raceSelect[buttonType][classIndex], Point<int16_t>(95 + (i * 126), 420));
		}

		buttons[BT_BACK] = std::make_unique<MapleButton>(login["Common"]["BtStart"], Point<int16_t>(0, 505));
		buttons[BT_CREATE] = std::make_unique<MapleButton>(raceSelect["make"], Point<int16_t>(59, 344));
		buttons[BT_PAGE_L] = std::make_unique<MapleButton>(raceSelect["leftArrow"], Point<int16_t>(41, 448));
		buttons[BT_PAGE_L]->set_state(Button::State::DISABLED);
		buttons[BT_PAGE_R] = std::make_unique<MapleButton>(raceSelect["rightArrow"], Point<int16_t>(718, 448));
		buttons[BT_PAGE_R]->set_state(Button::State::DISABLED);

		change_class(BT_CLASS_SEL0);
		buttons[BT_CLASS_SEL0]->set_state(Button::State::PRESSED);

		position = { 0, 0 };
		dimension = { 800, 600 };
		active = true;
	}

	Button::State UICharCreation::button_pressed(uint16_t id)
	{
		switch (id)
		{
		case BT_BACK:
			active = false;

			if (auto charselect = UI::get().get_element<UICharSelect>())
				charselect->makeactive();

			return Button::State::PRESSED;
		case BT_CLASS_SEL0:
		case BT_CLASS_SEL1:
		case BT_CLASS_SEL2:
		case BT_CLASS_SEL3:
		case BT_CLASS_SEL4:
			buttons[BT_CLASS_SEL0]->set_state(Button::State::NORMAL);
			buttons[BT_CLASS_SEL1]->set_state(Button::State::NORMAL);
			buttons[BT_CLASS_SEL2]->set_state(Button::State::NORMAL);
			buttons[BT_CLASS_SEL3]->set_state(Button::State::NORMAL);
			buttons[BT_CLASS_SEL4]->set_state(Button::State::NORMAL);

			sprites._Pop_back_n(4);
			change_class(id);

			return Button::State::PRESSED;
		case BT_CREATE:
			if (activeClass == Classes::EVAN || activeClass == Classes::DUAL_BLADE)
			{
				UI::get().emplace<UIClassConfirm>(true, (uint16_t)0, (int8_t)0);
			}
			else
			{
				uint16_t cjob = 1;

				if (activeClass == Classes::ARAN)
					cjob = 2;
				else if (activeClass == Classes::EXPLORERS)
					cjob = 1;
				else if (activeClass == Classes::CYGNUS)
					cjob = 0;

				UI::get().emplace<UIClassConfirm>(false, cjob, activeClass);
			}

			return Button::State::NORMAL;
		default:
			return Button::State::NORMAL;
		}
	}

	Cursor::State UICharCreation::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		return UIElement::send_cursor(clicked, cursorpos);
	}

	void UICharCreation::send_naming_result(bool used)
	{
		if (auto arancreation = UI::get().get_element<UIClassCreation>())
			arancreation->send_naming_result(used);
	}

	void UICharCreation::change_class(uint8_t id)
	{
		int8_t classIndex = classMap.at(id);
		std::string classId = std::to_string(classIndex);

		sprites.emplace_back(raceSelect["Back0"][classId]["0"], Point<int16_t>(0, -10));
		sprites.emplace_back(raceSelect["Back1"]["0"]["0"], Point<int16_t>(0, -10));
		sprites.emplace_back(raceSelect["Back2"][classId]["0"], Point<int16_t>(0, -10));

		if (classMap[id] == Classes::ARAN || classMap[id] == Classes::EVAN)
			sprites.emplace_back(raceSelect["Back3"]["3"]["0"], Point<int16_t>(0, -10));
		else if (classMap[id] == Classes::CYGNUS)
			sprites.emplace_back(raceSelect["Back3"]["2"]["0"], Point<int16_t>(0, -10));
		else
			sprites.emplace_back(raceSelect["Back3"]["1"]["0"], Point<int16_t>(0, -10));

		activeClass = classIndex;
	}

	void UICharCreation::draw(float alpha) const
	{
		UIElement::draw(alpha);

		version.draw(position + Point<int16_t>(707, -9));
	}

	void UICharCreation::update()
	{
		UIElement::update();
	}
}
