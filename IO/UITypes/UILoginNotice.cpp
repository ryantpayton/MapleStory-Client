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
#include "UILoginNotice.h"
#include "UIClassCreation.h"
#include "UICharCreation.h"
#include "UICharSelect.h"

#include "../UI.h"
#include "../Components/MapleButton.h"

#include "../../Graphics/Sprite.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UILoginNotice::UILoginNotice(int8_t id)
	{
		nl::node notice = nl::nx::ui["Login.img"]["Notice"];

		int8_t back = (id == BLOCKED_ID || id == INCORRECT_PIC || id == NAME_IN_USE || id == ILLEGAL_NAME) ? 1 : 0;
		background = notice["backgrnd"][back];
		text = { notice["text"][id], { 17, 13 } };

		buttons[BT_OK] = std::make_unique<MapleButton>(notice["BtYes"], Point<int16_t>(100, 106));

		position = { 275, 199 };
		dimension = { 362, 219 };
		active = true;
	}

	void UILoginNotice::draw(float alpha) const
	{
		background.draw(position);
		text.draw(position, alpha);

		UIElement::draw(alpha);
	}

	Button::State UILoginNotice::button_pressed(uint16_t id)
	{
		active = false;
		return Button::PRESSED;
	}

	void UILoginNotice::send_key(int32_t keycode, bool pressed)
	{
		if (pressed)
			if (keycode == KeyAction::ESCAPE || keycode == KeyAction::RETURN)
				active = false;
	}

	UIQuitConfirm::UIQuitConfirm()
	{
		nl::node notice = nl::nx::ui["Login.img"]["Notice"];

		background = notice["backgrnd"]["0"];
		text = { notice["text"][UILoginNotice::Message::CONFIRM_EXIT], { 17, 13 } };

		buttons[BT_OK] = std::make_unique<MapleButton>(notice["BtYes"], Point<int16_t>(70, 106));
		buttons[BT_CANCEL] = std::make_unique<MapleButton>(notice["BtNo"], Point<int16_t>(130, 106));

		position = { 275, 199 };
		dimension = { 362, 219 };
		active = true;
	}

	void UIQuitConfirm::draw(float alpha) const
	{
		background.draw(position);
		text.draw(position, alpha);

		UIElement::draw(alpha);
	}

	Button::State UIQuitConfirm::button_pressed(uint16_t id)
	{
		if (id == BT_OK)
			UI::get().quit();

		active = false;
		return Button::PRESSED;
	}

	void UIQuitConfirm::send_key(int32_t keycode, bool pressed)
	{
		if (pressed)
		{
			if (keycode == KeyAction::ESCAPE)
			{
				active = false;
			}
			else if (keycode == KeyAction::RETURN)
			{
				UI::get().quit();
				active = false;
			}
		}
	}

	UIClassConfirm::UIClassConfirm(bool u, uint16_t r, int8_t classMap) : unavailable(u), race(r)
	{
		nl::node notice = nl::nx::ui["Login.img"]["RaceSelect_new"];
		nl::node type = unavailable ? notice["deny"] : notice["confirm"];

		background = type["backgrnd"];

		if (!unavailable)
			text = { type["race"][classMap], { 17, 13 } };

		if (unavailable)
		{
			buttons[BT_OK] = std::make_unique<MapleButton>(type["BtOK"], Point<int16_t>(100, 105));
		}
		else
		{
			buttons[BT_OK] = std::make_unique<MapleButton>(type["BtOK"], Point<int16_t>(62, 107));
			buttons[BT_CANCEL] = std::make_unique<MapleButton>(type["BtCancel"], Point<int16_t>(137, 107));
		}

		position = { 286, 179 };
		dimension = { 362, 219 };
		active = true;
	}

	void UIClassConfirm::draw(float alpha) const
	{
		background.draw(position);

		if (!unavailable)
			text.draw(position + Point<int16_t>(72, -3), alpha);

		UIElement::draw(alpha);
	}

	Button::State UIClassConfirm::button_pressed(uint16_t id)
	{
		if (!unavailable && id == BT_OK)
			create_class();

		active = false;
		return Button::PRESSED;
	}

	void UIClassConfirm::send_key(int32_t keycode, bool pressed)
	{
		if (pressed)
		{
			if (keycode == KeyAction::ESCAPE)
			{
				active = false;
			}
			else if (keycode == KeyAction::RETURN)
			{
				if (!unavailable)
					create_class();

				active = false;
			}
		}
	}

	void UIClassConfirm::create_class()
	{
		if (auto charcreation = UI::get().get_element<UICharCreation>())
			charcreation->deactivate();

		UI::get().emplace<UIClassCreation>(race);
	}

	UIKeySelect::UIKeySelect()
	{
		nl::node keyType = nl::nx::ui["UIWindow2.img"]["KeyConfig"]["KeyType"];

		background = keyType["backgrnd"];

		buttons[BT_KEY_TYPE_A] = std::make_unique<MapleButton>(keyType["btTypeA"]);
		buttons[BT_KEY_TYPE_B] = std::make_unique<MapleButton>(keyType["btTypeB"], Point<int16_t>(1, 1));

		position = { 181, 135 };
		dimension = { 362, 219 };
		active = true;
	}

	void UIKeySelect::draw(float alpha) const
	{
		background.draw(position);

		UIElement::draw(alpha);
	}

	Button::State UIKeySelect::button_pressed(uint16_t id)
	{
		uint8_t type = (id == BT_KEY_TYPE_A) ? 0 : 1;

		if (id == BT_KEY_TYPE_A)
			buttons[BT_KEY_TYPE_B]->set_state(Button::State::DISABLED);
		else
			buttons[BT_KEY_TYPE_A]->set_state(Button::State::DISABLED);

		UI::get().emplace<UIKeyConfirm>(type);

		return Button::State::DISABLED;
	}

	UIKeyConfirm::UIKeyConfirm(uint8_t type)
	{
		nl::node notice = nl::nx::ui["UIWindow2.img"]["KeyConfig"]["KeyType"]["alert"];

		background = (type == 0) ? notice["default"] : notice["alternate"];

		buttons[BT_OK] = std::make_unique<MapleButton>(notice["btOk"]);

		position = { 276, 219 };
		dimension = { 362, 219 };
		active = true;
	}

	void UIKeyConfirm::draw(float alpha) const
	{
		background.draw(position);

		UIElement::draw(alpha);
	}

	void UIKeyConfirm::confirm_action()
	{
		active = false;

		UI::get().remove(UIElement::Type::LOGINNOTICE_CONFIRM);
		UI::get().remove(UIElement::Type::LOGINNOTICE);
		UI::get().remove(UIElement::Type::CLASSCREATION);
		UI::get().remove(UIElement::Type::CHARCREATION);

		if (auto charselect = UI::get().get_element<UICharSelect>())
			charselect->makeactive();
	}

	Button::State UIKeyConfirm::button_pressed(uint16_t id)
	{
		confirm_action();

		return Button::State::NORMAL;
	}

	void UIKeyConfirm::send_key(int32_t keycode, bool pressed)
	{
		if (pressed && keycode == KeyAction::RETURN)
			confirm_action();
	}
}