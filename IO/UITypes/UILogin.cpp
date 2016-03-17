/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 Daniel Allendorf                                        //
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
#include "IO\UI.h"
#include "Net\Session.h"
#include "Configuration.h"
#include "Audio\AudioPlayer.h"
#include "Net\Packets\LoginPackets.h"
#include "IO\Components\MapleButton.h"
#include "Graphics\Sprite.h"
#include "nlnx\nx.hpp"
#include "nlnx\audio.hpp"

namespace IO
{
	using Audio::AudioPlayer;
	using Graphics::DrawArgument;
	using Net::Session;

	UILogin::UILogin()
	{
		AudioPlayer::get().playbgm("BgmUI.img/Title");

		node title = nl::nx::ui["Login.img"]["Title"];
		node common = nl::nx::ui["Login.img"]["Common"];

		using::Graphics::Sprite;
		sprites.push_back(Sprite(title["11"], Point<int16_t>(410, 300)));
		sprites.push_back(Sprite(title["35"], Point<int16_t>(410, 260)));
		sprites.push_back(Sprite(title["Logo"], Point<int16_t>(410, 130)));
		sprites.push_back(Sprite(title["signboard"], Point<int16_t>(410, 300)));
		sprites.push_back(Sprite(common["frame"], Point<int16_t>(400, 290)));

		// I prefer the title without this.
		/*auto effectpos = Point<int16_t>(420, -50);
		node effect = title["effect"];
		for (node sub : effect)
		{
			auto sprite = Sprite(sub, DrawArgument(effectpos, true, 0.5f));
			sprites.push_back(sprite);
		}*/

		buttons[BT_LOGIN] = unique_ptr<Button>(new MapleButton(title["BtLogin"], Point<int16_t>(475, 248)));
		buttons[BT_REGISTER] = unique_ptr<Button>(new MapleButton(title["BtNew"], Point<int16_t>(309, 320)));
		buttons[BT_HOMEPAGE] = unique_ptr<Button>(new MapleButton(title["BtHomePage"], Point<int16_t>(382, 320)));
		buttons[BT_PASSLOST] = unique_ptr<Button>(new MapleButton(title["BtPasswdLost"], Point<int16_t>(470, 300)));
		buttons[BT_QUIT] = unique_ptr<Button>(new MapleButton(title["BtQuit"], Point<int16_t>(455, 320)));
		buttons[BT_IDLOST] = unique_ptr<Button>(new MapleButton(title["BtLoginIDLost"], Point<int16_t>(395, 300)));
		buttons[BT_SAVEID] = unique_ptr<Button>(new MapleButton(title["BtLoginIDSave"], Point<int16_t>(325, 300)));

		checkbox[false] = Texture(title["check"]["0"]);
		checkbox[true] = Texture(title["check"]["1"]);

		account = Textfield(
			Text::A13M, Text::LEFT, Text::WHITE, 
			rectangle2d<int16_t>(
			Point<int16_t>(315, 249), 
			Point<int16_t>(465, 273)), 12
			);
		account.setkey(Keyboard::TAB, [&](){
			account.setstate(Textfield::NORMAL);
			password.setstate(Textfield::FOCUSED);
		});
		accountbg = Texture(title["ID"]);

		password = Textfield(
			Text::A13M, Text::LEFT, Text::WHITE,
			rectangle2d<int16_t>(
			Point<int16_t>(315, 275),
			Point<int16_t>(465, 299)), 12
			);
		password.setkey(Keyboard::TAB, [&](){
			password.setstate(Textfield::NORMAL);
			account.setstate(Textfield::FOCUSED);
		});
		password.setonreturn([&](string msg){
			login();
		});
		passwordbg = Texture(title["PW"]);
		password.setcrypt('*');

		saveid = Configuration::get().getbool(Settings::SAVE_LOGIN);
		if (saveid)
		{
			account.settext(Configuration::get().getsetting(Settings::ACCOUNT));
			password.setstate(Textfield::FOCUSED);
		}
		else
		{
			account.setstate(Textfield::FOCUSED);
		}

		position = Point<int16_t>(0, 0);
		dimension = Point<int16_t>(800, 600);
		active = true;
	}

	void UILogin::draw(float inter) const
	{
		UIElement::draw(inter);

		account.draw(position);
		password.draw(position);

		using::Graphics::DrawArgument;
		if (account.getstate() == Textfield::NORMAL && account.gettext().size() == 0)
			accountbg.draw(DrawArgument(310, 249));
		if (password.getstate() == Textfield::NORMAL && password.gettext().size() == 0)
			passwordbg.draw(DrawArgument(310, 275));

		checkbox.at(saveid).draw(DrawArgument(position + Point<int16_t>(313, 304)));
	}

	void UILogin::update()
	{
		UIElement::update();

		account.update(position);
		password.update(position);
	}

	void UILogin::buttonpressed(uint16_t id)
	{
		switch (id)
		{
		case BT_LOGIN:
			login();
			return;
		case BT_QUIT:
			Session::get().disconnect();
			return;
		case BT_SAVEID:
			saveid = !saveid;
			Configuration::get().setbool(Settings::SAVE_LOGIN, saveid);
			buttons[BT_SAVEID]->setstate(Button::MOUSEOVER);
			return;
		}
	}

	void UILogin::login()
	{
		UI::get().disable();
		UI::get().focustextfield(nullptr);
		UI::get().add(ElementLoginwait());

		account.setstate(Textfield::NORMAL);
		password.setstate(Textfield::NORMAL);

		buttons[BT_LOGIN]->setstate(Button::MOUSEOVER);

		using Net::LoginPacket;
		Session::get().dispatch(LoginPacket(account.gettext(), password.gettext()));
	}

	Cursor::State UILogin::sendmouse(bool clicked, Point<int16_t> cursorpos)
	{
		if (account.getstate() == Textfield::NORMAL)
		{
			Cursor::State astate = account.sendcursor(cursorpos, clicked);
			if (astate != Cursor::IDLE)
			{
				return astate;
			}
		}

		if (password.getstate() == Textfield::NORMAL)
		{
			Cursor::State pstate = password.sendcursor(cursorpos, clicked);
			if (pstate != Cursor::IDLE)
			{
				return pstate;
			}
		}
		return UIElement::sendmouse(clicked, cursorpos);
	}
}
