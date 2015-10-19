/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "LoginPacket.h"
#include "MapleButton.h"
#include "Sprite.h"
#include "nx.hpp"

using namespace nl;

namespace IO
{
	UILogin::UILogin(Session& ses, UI& u) : session(ses), ui(u)
	{
		node title = nx::ui["Login.img"]["Title"];
		node common = nx::ui["Login.img"]["Common"];

		graphics.add(new Sprite(title["11"], vector2d<int>(410, 300)));
		graphics.add(new Sprite(title["35"], vector2d<int>(410, 260)));
		graphics.add(new Sprite(title["Logo"], vector2d<int>(410, 130)));
		graphics.add(new Sprite(title["signboard"], vector2d<int>(410, 300)));
		graphics.add(new Sprite(common["frame"], vector2d<int>(400, 290)));

		buttons.add(BT_LOGIN, new MapleButton(title["BtLogin"], vector2d<int>(475, 248)));
		buttons.add(BT_REGISTER, new MapleButton(title["BtNew"], vector2d<int>(309, 320)));
		buttons.add(BT_HOMEPAGE, new MapleButton(title["BtHomePage"], vector2d<int>(382, 320)));
		buttons.add(BT_PASSLOST, new MapleButton(title["BtPasswdLost"], vector2d<int>(470, 300)));
		buttons.add(BT_QUIT, new MapleButton(title["BtQuit"], vector2d<int>(455, 320)));
		buttons.add(BT_IDLOST, new MapleButton(title["BtLoginIDLost"], vector2d<int>(395, 300)));
		buttons.add(BT_SAVEID, new MapleButton(title["BtLoginIDSave"], vector2d<int>(325, 300)));

		//string defaultacc = Game::getconfig()->defaultacc;
		//saveid = Game::getconfig()->saveid;
		saveid = false;

		checkbox.add(false, new Texture(title["check"]["0"]));
		checkbox.add(true, new Texture(title["check"]["1"]));

		account = new Textfield(DWF_TEXTFIELD, TXC_WHITE, vector2d<int>(315, 249), 12);
		accountbg = new Texture(title["ID"]);

		password = new Textfield(DWF_TEXTFIELD, TXC_WHITE, vector2d<int>(315, 275), 12);
		passwordbg = new Texture(title["PW"]);
		password->setcrypt('*');

		ui.gethandler()->focustext(saveid ? password : account);

		position = vector2d<int>(0, 0);
		dimension = vector2d<int>(800, 600);
		active = true;
	}

	UILogin::~UILogin()
	{
		delete account;
		delete password;
		delete accountbg;
		delete passwordbg;
	}

	void UILogin::draw()
	{
		UIElement::draw();

		if (active)
		{
			account->draw(position);
			password->draw(position);

			if (account->getstate() == TXFS_NORMAL && account->gettext().size() == 0)
			{
				accountbg->draw(PosArgument(vector2d<int>(310, 249)));
			}
			if (password->getstate() == TXFS_NORMAL && password->gettext().size() == 0)
			{
				passwordbg->draw(PosArgument(vector2d<int>(310, 275)));
			}

			checkbox.get(saveid)->draw(PosArgument(position + vector2d<int>(313, 304)));
		}
	}

	void UILogin::update(short dpf)
	{
		if (active)
		{
			UIElement::update(dpf);

			account->update(dpf);
			password->update(dpf);
		}
	}

	void UILogin::buttonpressed(short id)
	{
		switch (id)
		{
		case BT_LOGIN:
			ui.disable();
			ui.gethandler()->focustext(0);
			buttons.get(BT_LOGIN)->setstate(BTS_MOUSEOVER);
			//ui->add(UI_LOGINWAIT);
			session.dispatch(LoginPacket(account->gettext(), password->gettext()));
			return;
		case BT_QUIT:
			session.disconnect();
			return;
		case BT_SAVEID:
			saveid = !saveid;
			//Game::getconfig()->saveid = saveid;
			buttons.get(BT_SAVEID)->setstate(BTS_MOUSEOVER);
			return;
		}
	}

	Mousestate UILogin::sendmouse(bool down, vector2d<int> pos)
	{
		Mousestate ret = UIElement::sendmouse(down, pos);

		if (account->bounds(position).contains(pos))
		{
			if (down)
			{
				ui.gethandler()->focustext(account);
			}
			else if (account->getstate() == TXFS_NORMAL)
			{
				ret = MST_CANCLICK;
			}
		}
		else if (password->bounds(position).contains(pos))
		{
			if (down)
			{
				ui.gethandler()->focustext(password);
			}
			else if (password->getstate() == TXFS_NORMAL)
			{
				ret = MST_CANCLICK;
			}
		}
		else if (down)
		{
			ui.gethandler()->focustext(0);
		}

		return ret;
	}
}
