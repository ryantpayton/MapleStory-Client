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
#include "UILogin.h"
#include "UILoginwait.h"

#include "../UI.h"
#include "../Components/MapleButton.h"

#include "../../Configuration.h"
#include "../../Audio/Audio.h"
#include "../../Graphics/Sprite.h"
#include "../../Net/Packets/LoginPackets.h"
#include "../../IO/UITypes/UILoginNotice.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UILogin::UILogin()
	{
		Music("BgmUI.img/Title").play();

		version = { Text::Font::A11M, Text::Alignment::LEFT, Text::Color::DARKSTEEL, "Ver. 203.3" };

		nl::node map = nl::nx::map["Back"]["login.img"];
		nl::node obj = nl::nx::map["Obj"]["login.img"];
		nl::node back = map["back"];
		nl::node ani = map["ani"];
		nl::node title = nl::nx::ui["Login.img"]["Title"];
		nl::node common = nl::nx::ui["Login.img"]["Common"];

		sprites.emplace_back(back["11"], Point<int16_t>(400, 290)); // From v203.3
		sprites.emplace_back(ani["17"], Point<int16_t>(129, 273)); // From v167
		sprites.emplace_back(ani["18"], Point<int16_t>(306, 242)); // From v167
		sprites.emplace_back(ani["19"], Point<int16_t>(379, 197)); // From v167
		sprites.emplace_back(back["35"], Point<int16_t>(399, 250)); // From v203.3
		sprites.emplace_back(ani["16"], Point<int16_t>(394, 163)); // From v167
		sprites.emplace_back(title["signboard"], Point<int16_t>(391, 320));
		sprites.emplace_back(common["frame"], Point<int16_t>(399, 289));

		buttons[BT_LOGIN] = std::make_unique<MapleButton>(title["BtLogin"], Point<int16_t>(454, 269));
		buttons[BT_SAVEID] = std::make_unique<MapleButton>(title["BtLoginIDSave"], Point<int16_t>(303, 322));
		buttons[BT_IDLOST] = std::make_unique<MapleButton>(title["BtLoginIDLost"], Point<int16_t>(375, 322));
		buttons[BT_PASSLOST] = std::make_unique<MapleButton>(title["BtPasswdLost"], Point<int16_t>(447, 322));
		buttons[BT_REGISTER] = std::make_unique<MapleButton>(title["BtNew"], Point<int16_t>(291, 342));
		buttons[BT_HOMEPAGE] = std::make_unique<MapleButton>(title["BtHomePage"], Point<int16_t>(363, 342));
		buttons[BT_QUIT] = std::make_unique<MapleButton>(title["BtQuit"], Point<int16_t>(435, 342));

		checkbox[false] = title["check"]["0"];
		checkbox[true] = title["check"]["1"];

		account = { Text::A13M, Text::LEFT, Text::WHITE, { { 296, 269 }, { 446, 293 } }, 12 };

		account.set_key_callback
		(
			KeyAction::TAB, [&]
			{
				account.set_state(Textfield::NORMAL);
				password.set_state(Textfield::FOCUSED);
			}
		);

		account.set_enter_callback
		(
			[&](std::string msg)
			{
				if (msg.size() > 0)
				login();
			}
		);

		accountbg = title["ID"];

		password = { Text::A13M, Text::LEFT, Text::WHITE, { { 296, 295 }, { 446, 319 } }, 12 };

		password.set_key_callback
		(
			KeyAction::TAB, [&]
			{
				password.set_state(Textfield::NORMAL);
				account.set_state(Textfield::FOCUSED);
			}
		);

		password.set_enter_callback
		(
			[&](std::string msg)
			{
				if (msg.size() > 0)
				login();
			}
		);

		password.set_cryptchar('*');
		passwordbg = title["PW"];

		saveid = Setting<SaveLogin>::get().load();

		if (saveid)
		{
			account.change_text(Setting<DefaultAccount>::get().load());
			password.set_state(Textfield::FOCUSED);
		}
		else
		{
			account.set_state(Textfield::FOCUSED);
		}

		position = { 0, 0 };
		dimension = { 800, 600 };
		active = true;
	}

	void UILogin::draw(float alpha) const
	{
		UIElement::draw(alpha);

		version.draw(position + Point<int16_t>(707, -9));
		account.draw(position);
		password.draw(position);

		if (account.get_state() == Textfield::NORMAL && account.empty())
			accountbg.draw({ position + Point<int16_t>(291, 269) });

		if (password.get_state() == Textfield::NORMAL && password.empty())
			passwordbg.draw({ position + Point<int16_t>(291, 295) });

		checkbox[saveid].draw({ position + Point<int16_t>(291, 325) });
	}

	void UILogin::update()
	{
		UIElement::update();

		account.update(position);
		password.update(position);
	}

	void UILogin::login()
	{
		if (account.get_text() == "") {
			UI::get().emplace<UILoginNotice>(UILoginNotice::Message::NOT_REGISTERED);
			UI::get().enable();
			return;
		}

		if (password.get_text().length() <= 4) {
			UI::get().emplace<UILoginNotice>(UILoginNotice::Message::WRONG_PASSWORD);
			UI::get().enable();
			return;
		}

		UI::get().disable();
		UI::get().emplace<UILoginwait>();

		account.set_state(Textfield::NORMAL);
		password.set_state(Textfield::NORMAL);

		LoginPacket(
			account.get_text(),
			password.get_text()
		).dispatch();
	}

	Button::State UILogin::button_pressed(uint16_t id)
	{
		switch (id)
		{
		case BT_LOGIN:
			login();
			return Button::NORMAL;
		case BT_QUIT:
			UI::get().quit();
			return Button::PRESSED;
		case BT_SAVEID:
			saveid = !saveid;
			Setting<SaveLogin>::get().save(saveid);
			return Button::MOUSEOVER;
		default:
			return Button::NORMAL;
		}
	}

	Cursor::State UILogin::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (Cursor::State new_state = account.send_cursor(cursorpos, clicked))
			return new_state;

		if (Cursor::State new_state = password.send_cursor(cursorpos, clicked))
			return new_state;

		return UIElement::send_cursor(clicked, cursorpos);
	}
}