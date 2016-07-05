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
#include "UILogin.h"
#include "UILoginwait.h"

#include "../UI.h"
#include "../Components/MapleButton.h"

#include "../../Configuration.h"
#include "../../Audio/Audio.h"
#include "../../Graphics/Sprite.h"
#include "../../Net/Packets/LoginPackets.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UILogin::UILogin()
	{
		Music("BgmUI.img/Title")
			.play();

		nl::node title = nl::nx::ui["Login.img"]["Title"];
		nl::node common = nl::nx::ui["Login.img"]["Common"];

		sprites.emplace_back(title["11"], Point<int16_t>(410, 300));
		sprites.emplace_back(title["35"], Point<int16_t>(410, 260));
		sprites.emplace_back(title["Logo"], Point<int16_t>(410, 130));
		sprites.emplace_back(title["signboard"], Point<int16_t>(410, 300));
		sprites.emplace_back(common["frame"], Point<int16_t>(400, 290));

		// I prefer the title without this.
		/*auto effectpos = Point<int16_t>(420, -50);
		node effect = title["effect"];
		for (node sub : effect)
		{
			auto sprite = Sprite(sub, DrawArgument(effectpos, true, 0.5f));
			sprites.push_back(sprite);
		}*/

		buttons[BT_LOGIN] = std::make_unique<MapleButton>(title["BtLogin"], Point<int16_t>(475, 248));
		buttons[BT_REGISTER] = std::make_unique<MapleButton>(title["BtNew"], Point<int16_t>(309, 320));
		buttons[BT_HOMEPAGE] = std::make_unique<MapleButton>(title["BtHomePage"], Point<int16_t>(382, 320));
		buttons[BT_PASSLOST] = std::make_unique<MapleButton>(title["BtPasswdLost"], Point<int16_t>(470, 300));
		buttons[BT_QUIT] = std::make_unique<MapleButton>(title["BtQuit"], Point<int16_t>(455, 320));
		buttons[BT_IDLOST] = std::make_unique<MapleButton>(title["BtLoginIDLost"], Point<int16_t>(395, 300));
		buttons[BT_SAVEID] = std::make_unique<MapleButton>(title["BtLoginIDSave"], Point<int16_t>(325, 300));

		checkbox[false] = title["check"]["0"];
		checkbox[true] = title["check"]["1"];

		account = { Text::A13M, Text::LEFT, Text::WHITE,{ { 315, 249 },{ 465, 273 } }, 12 };
		account.set_key_callback(KeyAction::TAB, [&]{
			account.set_state(Textfield::NORMAL);
			password.set_state(Textfield::FOCUSED);
		});
		account.set_enter_callback([&](std::string) {
			login();
		});
		accountbg = title["ID"];

		password = { Text::A13M, Text::LEFT, Text::WHITE, { {315, 275}, {465, 299} }, 12 };
		password.set_key_callback(KeyAction::TAB, [&]{
			password.set_state(Textfield::NORMAL);
			account.set_state(Textfield::FOCUSED);
		});
		password.set_enter_callback([&](std::string){
			login();
		});
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

		account.draw(position);
		password.draw(position);

		if (account.get_state() == Textfield::NORMAL && account.empty())
		{
			accountbg.draw({ position + Point<int16_t>(310, 249) });
		}

		if (password.get_state() == Textfield::NORMAL && password.empty())
		{
			passwordbg.draw({ position + Point<int16_t>(310, 275) });
		}

		checkbox[saveid]
			.draw({ position + Point<int16_t>(313, 304) });
	}

	void UILogin::update()
	{
		UIElement::update();

		account.update(position);
		password.update(position);
	}

	void UILogin::login()
	{
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
			return Button::PRESSED;
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
