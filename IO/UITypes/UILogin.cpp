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
#pragma once
#include "UILogin.h"
#include "UILoginwait.h"
#include "Net\Packets\LoginPackets83.h"
#include "IO\Components\MapleButton.h"
#include "Graphics\Sprite.h"
#include "nlnx\nx.hpp"
#include "nlnx\audio.hpp"

#define button_ptr unique_ptr<Button>

namespace IO
{
	UILogin::UILogin(Session& ses, UI& u, Configuration& cfg) : session(ses), ui(u), config(cfg)
	{
		node title = nl::nx::ui["Login.img"]["Title"];
		node common = nl::nx::ui["Login.img"]["Common"];

		using::Graphics::Sprite;
		sprites.push_back(Sprite(title["11"], vector2d<int32_t>(410, 300)));
		sprites.push_back(Sprite(title["35"], vector2d<int32_t>(410, 260)));
		sprites.push_back(Sprite(title["Logo"], vector2d<int32_t>(410, 130)));
		sprites.push_back(Sprite(title["signboard"], vector2d<int32_t>(410, 300)));
		sprites.push_back(Sprite(common["frame"], vector2d<int32_t>(400, 290)));

		buttons[BT_LOGIN] = button_ptr(new MapleButton(title["BtLogin"], vector2d<int32_t>(475, 248)));
		buttons[BT_REGISTER] = button_ptr(new MapleButton(title["BtNew"], vector2d<int32_t>(309, 320)));
		buttons[BT_HOMEPAGE] = button_ptr(new MapleButton(title["BtHomePage"], vector2d<int32_t>(382, 320)));
		buttons[BT_PASSLOST] = button_ptr(new MapleButton(title["BtPasswdLost"], vector2d<int32_t>(470, 300)));
		buttons[BT_QUIT] = button_ptr(new MapleButton(title["BtQuit"], vector2d<int32_t>(455, 320)));
		buttons[BT_IDLOST] = button_ptr(new MapleButton(title["BtLoginIDLost"], vector2d<int32_t>(395, 300)));
		buttons[BT_SAVEID] = button_ptr(new MapleButton(title["BtLoginIDSave"], vector2d<int32_t>(325, 300)));

		checkbox[false] = Texture(title["check"]["0"]);
		checkbox[true] = Texture(title["check"]["1"]);

		account = Textfield(Textlabel::DWF_TEXTFIELD, Textlabel::TXC_WHITE, vector2d<int32_t>(315, 249), 12);
		accountbg = Texture(title["ID"]);

		password = Textfield(Textlabel::DWF_TEXTFIELD, Textlabel::TXC_WHITE, vector2d<int32_t>(315, 275), 12);
		passwordbg = Texture(title["PW"]);
		password.setcrypt('*');

		saveid = config.getbool("SaveLogin");
		if (saveid)
		{
			account.settext(config.getsetting("Account"));
			ui.getkeyboard().focustarget(&password);
			password.setstate(Textfield::FOCUSED);
		}
		else
		{
			ui.getkeyboard().focustarget(&account);
			account.setstate(Textfield::FOCUSED);
		}

		position = vector2d<int32_t>(0, 0);
		dimension = vector2d<int32_t>(800, 600);
		active = true;
	}

	void UILogin::draw(float inter) const
	{
		UIElement::draw(inter);

		if (active)
		{
			account.draw(position);
			password.draw(position);

			using::Graphics::DrawArgument;
			if (account.getstate() == Textfield::NORMAL && account.gettext().size() == 0)
			{
				accountbg.draw(DrawArgument(310, 249));
			}
			if (password.getstate() == Textfield::NORMAL && password.gettext().size() == 0)
			{
				passwordbg.draw(DrawArgument(310, 275));
			}
			checkbox.at(saveid).draw(DrawArgument(position + vector2d<int32_t>(313, 304)));
		}
	}

	void UILogin::update()
	{
		if (active)
		{
			UIElement::update();

			account.update();
			password.update();
		}
	}

	void UILogin::buttonpressed(uint16_t id)
	{
		switch (id)
		{
		case BT_LOGIN:
			ui.disable();
			ui.getkeyboard().focustarget(nullptr);
			account.setstate(Textfield::NORMAL);
			password.setstate(Textfield::NORMAL);
			buttons[BT_LOGIN]->setstate(Button::MOUSEOVER);
			ui.add(ElementLoginwait());

			using::Net::LoginPacket83;
			session.dispatch(LoginPacket83(account.gettext(), password.gettext()));
			return;
		case BT_QUIT:
			session.disconnect();
			return;
		case BT_SAVEID:
			saveid = !saveid;
			config.setbool("SaveLogin", saveid);
			buttons[BT_SAVEID]->setstate(Button::MOUSEOVER);
			return;
		}
	}

	Mousestate UILogin::sendmouse(bool down, vector2d<int32_t> pos)
	{
		Mousestate ret = UIElement::sendmouse(down, pos);

		if (account.bounds(position).contains(pos))
		{
			if (down)
			{
				ui.getkeyboard().focustarget(&account);
				account.setstate(Textfield::FOCUSED);
				password.setstate(Textfield::NORMAL);
			}
			else if (account.getstate() == Textfield::NORMAL)
			{
				ret = MST_CANCLICK;
			}
		}
		else if (password.bounds(position).contains(pos))
		{
			if (down)
			{
				ui.getkeyboard().focustarget(&password);
				password.setstate(Textfield::FOCUSED);
				account.setstate(Textfield::NORMAL);
			}
			else if (password.getstate() == Textfield::NORMAL)
			{
				ret = MST_CANCLICK;
			}
		}
		else if (down)
		{
			ui.getkeyboard().focustarget(nullptr);
			account.setstate(Textfield::NORMAL);
			password.setstate(Textfield::NORMAL);
		}

		return ret;
	}
}
