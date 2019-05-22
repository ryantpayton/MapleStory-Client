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

#include "../UI.h"

#include "../Components/MapleButton.h"
#include "../Audio/Audio.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UILoginNotice::UILoginNotice(uint16_t message, std::function<void()> okhandler) : okhandler(okhandler)
	{
		nl::node Notice = nl::nx::ui["Login.img"]["Notice"];
		nl::node backgrnd;

		switch (message)
		{
		case jrc::UILoginNotice::NAME_IN_USE:
		case jrc::UILoginNotice::ILLEGAL_NAME:
		case jrc::UILoginNotice::BLOCKED_ID:
		case jrc::UILoginNotice::INCORRECT_PIC:
			backgrnd = Notice["backgrnd"]["1"];
			break;
		default:
			backgrnd = Notice["backgrnd"]["0"];
			break;
		}

		sprites.emplace_back(backgrnd);
		sprites.emplace_back(Notice["text"][message], Point<int16_t>(17, 13));

		buttons[Buttons::YES] = std::make_unique<MapleButton>(Notice["BtYes"], Point<int16_t>(100, 106));

		position = Point<int16_t>(275, 199);
		dimension = Texture(backgrnd).get_dimensions();
		active = true;
	}

	UILoginNotice::UILoginNotice(uint16_t message) : UILoginNotice(message, []() {}) {}

	void UILoginNotice::draw(float alpha) const
	{
		UIElement::draw(alpha);
	}

	void UILoginNotice::send_key(int32_t keycode, bool pressed)
	{
		if (pressed)
			if (keycode == KeyAction::ESCAPE || keycode == KeyAction::RETURN)
				close();
	}

	void UILoginNotice::close()
	{
		active = false;

		okhandler();
	}

	Button::State UILoginNotice::button_pressed(uint16_t id)
	{
		close();

		return Button::State::NORMAL;
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

	Button::State UIQuitConfirm::button_pressed(uint16_t id)
	{
		if (id == BT_OK)
			UI::get().quit();

		active = false;
		return Button::PRESSED;
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

	Cursor::State UIClassConfirm::send_cursor(bool down, Point<int16_t> pos)
	{
		for (auto& btit : buttons)
		{
			if (btit.second->is_active() && btit.second->bounds(position).contains(pos))
			{
				if (btit.second->get_state() == Button::NORMAL)
				{
					Sound(Sound::BUTTONOVER).play();

					btit.second->set_state(Button::MOUSEOVER);
				}
				else if (btit.second->get_state() == Button::MOUSEOVER)
				{
					if (down)
					{
						Sound(Sound::BUTTONCLICK).play();

						btit.second->set_state(button_pressed(btit.first));
					}
				}
			}
			else if (btit.second->get_state() == Button::MOUSEOVER)
			{
				btit.second->set_state(Button::NORMAL);
			}
		}

		return Cursor::State::LEAF;
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

	Button::State UIClassConfirm::button_pressed(uint16_t id)
	{
		if (!unavailable && id == BT_OK)
			create_class();

		active = false;
		return Button::PRESSED;
	}

	void UIClassConfirm::create_class()
	{
		if (auto charcreation = UI::get().get_element<UICharCreation>())
			charcreation->deactivate();

		UI::get().emplace<UIClassCreation>(race);
	}

	UIKeySelect::UIKeySelect(std::function<void(bool)> oh, bool l) : okhandler(oh), login(l)
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

	void UIKeySelect::send_key(int32_t keycode, bool pressed)
	{
		if (pressed && !login)
			if (keycode == KeyAction::Id::ESCAPE || keycode == KeyAction::Id::RETURN)
				active = false;
	}

	Button::State UIKeySelect::button_pressed(uint16_t id)
	{
		bool alternate = (id == BT_KEY_TYPE_A) ? false : true;

		if (id == BT_KEY_TYPE_A)
			buttons[BT_KEY_TYPE_B]->set_state(Button::State::DISABLED);
		else
			buttons[BT_KEY_TYPE_A]->set_state(Button::State::DISABLED);

		auto onok = [&]()
		{
			active = false;
			okhandler(alternate);
		};

		UI::get().emplace<UIKeyConfirm>(alternate, onok, login);

		return Button::State::DISABLED;
	}

	UIKeyConfirm::UIKeyConfirm(bool alternate, std::function<void()> oh, bool l) : okhandler(oh), login(l)
	{
		nl::node notice = nl::nx::ui["UIWindow2.img"]["KeyConfig"]["KeyType"]["alert"];

		background = alternate ? notice["alternate"] : notice["default"];

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

	void UIKeyConfirm::send_key(int32_t keycode, bool pressed)
	{
		if (pressed)
		{
			if (keycode == KeyAction::RETURN)
			{
				confirm();
			}
			else if (!login && keycode == KeyAction::ESCAPE)
			{
				active = false;

				UI::get().remove(UIElement::LOGINNOTICE);
			}
		}
	}

	Button::State UIKeyConfirm::button_pressed(uint16_t id)
	{
		confirm();

		return Button::State::NORMAL;
	}

	void UIKeyConfirm::confirm()
	{
		active = false;
		okhandler();
	}
}