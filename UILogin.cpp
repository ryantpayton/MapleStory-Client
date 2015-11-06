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
#include "LoginPackets83.h"
#include "MapleButton.h"
#include "Sprite.h"
#include "UILoginwait.h"
#include "nx.hpp"
#include "audio.hpp"

namespace IO
{
	UILogin::UILogin(Window& wn, Session& ses, UI& u, Configuration& cfg) : window(wn), session(ses), ui(u), config(cfg)
	{
		node title = nl::nx::ui["Login.img"]["Title"];
		node common = nl::nx::ui["Login.img"]["Common"];

		using::Graphics::Sprite;
		graphics.add(new Sprite(title["11"], vector2d<int32_t>(410, 300)));
		graphics.add(new Sprite(title["35"], vector2d<int32_t>(410, 260)));
		graphics.add(new Sprite(title["Logo"], vector2d<int32_t>(410, 130)));
		graphics.add(new Sprite(title["signboard"], vector2d<int32_t>(410, 300)));
		graphics.add(new Sprite(common["frame"], vector2d<int32_t>(400, 290)));

		buttons.add(BT_LOGIN, new MapleButton(title["BtLogin"], vector2d<int32_t>(475, 248)));
		buttons.add(BT_REGISTER, new MapleButton(title["BtNew"], vector2d<int32_t>(309, 320)));
		buttons.add(BT_HOMEPAGE, new MapleButton(title["BtHomePage"], vector2d<int32_t>(382, 320)));
		buttons.add(BT_PASSLOST, new MapleButton(title["BtPasswdLost"], vector2d<int32_t>(470, 300)));
		buttons.add(BT_QUIT, new MapleButton(title["BtQuit"], vector2d<int32_t>(455, 320)));
		buttons.add(BT_IDLOST, new MapleButton(title["BtLoginIDLost"], vector2d<int32_t>(395, 300)));
		buttons.add(BT_SAVEID, new MapleButton(title["BtLoginIDSave"], vector2d<int32_t>(325, 300)));

		checkbox.add(false, new Texture(title["check"]["0"]));
		checkbox.add(true, new Texture(title["check"]["1"]));

		account = Textfield(DWF_TEXTFIELD, TXC_WHITE, vector2d<int32_t>(315, 249), 12);
		accountbg = new Texture(title["ID"]);

		password = Textfield(DWF_TEXTFIELD, TXC_WHITE, vector2d<int32_t>(315, 275), 12);
		passwordbg = new Texture(title["PW"]);
		password.setcrypt('*');

		saveid = config.getbool("SaveLogin");
		if (saveid)
		{
			account.settext(config.getsetting("Account"));
			window.getkeyboard().focustarget(&password);
			password.setstate(TXFS_FOCUSED);
		}
		else
		{
			window.getkeyboard().focustarget(&account);
			account.setstate(TXFS_FOCUSED);
		}

		position = vector2d<int32_t>(0, 0);
		dimension = vector2d<int32_t>(800, 600);
		active = true;
	}

	UILogin::~UILogin()
	{
		delete accountbg;
		delete passwordbg;
	}

	void UILogin::draw() const
	{
		UIElement::draw();

		if (active)
		{
			account.draw(position);
			password.draw(position);

			using::Graphics::PosArgument;
			if (account.getstate() == TXFS_NORMAL && account.gettext().size() == 0)
			{
				accountbg->draw(PosArgument(vector2d<int32_t>(310, 249)));
			}
			if (password.getstate() == TXFS_NORMAL && password.gettext().size() == 0)
			{
				passwordbg->draw(PosArgument(vector2d<int32_t>(310, 275)));
			}
			checkbox.get(saveid)->draw(PosArgument(position + vector2d<int32_t>(313, 304)));
		}
	}

	void UILogin::update(uint16_t dpf)
	{
		if (active)
		{
			UIElement::update(dpf);

			account.update(dpf);
			password.update(dpf);
		}
	}

	void UILogin::buttonpressed(uint16_t id)
	{
		switch (id)
		{
		case BT_LOGIN:
			ui.disable();
			window.getkeyboard().focustarget(0);
			account.setstate(TXFS_NORMAL);
			password.setstate(TXFS_NORMAL);
			buttons.get(BT_LOGIN)->setstate(BTS_MOUSEOVER);
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
			buttons.get(BT_SAVEID)->setstate(BTS_MOUSEOVER);
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
				window.getkeyboard().focustarget(&account);
				account.setstate(TXFS_FOCUSED);
				password.setstate(TXFS_NORMAL);
			}
			else if (account.getstate() == TXFS_NORMAL)
			{
				ret = MST_CANCLICK;
			}
		}
		else if (password.bounds(position).contains(pos))
		{
			if (down)
			{
				window.getkeyboard().focustarget(&password);
				password.setstate(TXFS_FOCUSED);
				account.setstate(TXFS_NORMAL);
			}
			else if (password.getstate() == TXFS_NORMAL)
			{
				ret = MST_CANCLICK;
			}
		}
		else if (down)
		{
			window.getkeyboard().focustarget(0);
			account.setstate(TXFS_NORMAL);
			password.setstate(TXFS_NORMAL);
		}

		return ret;
	}
}
