//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "UILogin.h"

#include "UILoginNotice.h"
#include "UILoginWait.h"

#include "../UI.h"

#include "../Components/MapleButton.h"
#include "../Components/TwoSpriteButton.h"

#include "../../Audio/Audio.h"

#include "../../Net/Packets/LoginPackets.h"

#include <windows.h>

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UILogin::UILogin() : UIElement(Point<int16_t>(0, 0), Point<int16_t>(1024, 768)), title_pos(Point<int16_t>(344, 246)), nexon(false)
	{
		LoginStartPacket().dispatch();

		std::string LoginMusicNewtro = Configuration::get().get_login_music_newtro();

		Music(LoginMusicNewtro).play();

		std::string version_text = Configuration::get().get_version();
		version = Text(Text::Font::A12B, Text::Alignment::LEFT, Color::Name::LEMONGRASS, "Ver. " + version_text);
		version_pos = nl::nx::UI["Login.img"]["Common"]["version"]["pos"];

		nl::node Login = nl::nx::UI["Login.img"];
		version_pos = Login["Common"]["version"]["pos"];

		nl::node Title_new = Login["Title_new"];
		capslock = Title_new["capslock"];

		nl::node check_src = Title_new["check"];
		check[false] = check_src["0"];
		check[true] = check_src["1"];

		sprites.emplace_back(nl::nx::Map001["Back"]["UI_login.img"]["back"]["0"], Point<int16_t>(512, 384));
		sprites.emplace_back(Title_new["backgrd"], title_pos);

		nl::node Tab = Title_new["Tab"];
		nl::node TabD = Tab["disabled"];
		nl::node TabE = Tab["enabled"];

		buttons[Buttons::BtLogin] = std::make_unique<MapleButton>(Title_new["BtLogin"], title_pos);
		buttons[Buttons::BtEmailSave] = std::make_unique<MapleButton>(Title_new["BtEmailSave"], title_pos);
		buttons[Buttons::BtEmailLost] = std::make_unique<MapleButton>(Title_new["BtEmailLost"], title_pos);
		buttons[Buttons::BtPasswdLost] = std::make_unique<MapleButton>(Title_new["BtPasswdLost"], title_pos);
		buttons[Buttons::BtNew] = std::make_unique<MapleButton>(Title_new["BtNew"], title_pos);
		buttons[Buttons::BtHomePage] = std::make_unique<MapleButton>(Title_new["BtHomePage"], title_pos);
		buttons[Buttons::BtQuit] = std::make_unique<MapleButton>(Title_new["BtQuit"], title_pos);
		buttons[Buttons::BtMapleID] = std::make_unique<TwoSpriteButton>(TabD["0"], TabE["0"], Point<int16_t>(344, 246));
		buttons[Buttons::BtNexonID] = std::make_unique<TwoSpriteButton>(TabD["1"], TabE["1"], Point<int16_t>(344, 246));

		if (nexon)
		{
			buttons[Buttons::BtNexonID]->set_state(Button::State::PRESSED);
			buttons[Buttons::BtMapleID]->set_state(Button::State::NORMAL);
		}
		else
		{
			buttons[Buttons::BtNexonID]->set_state(Button::State::NORMAL);
			buttons[Buttons::BtMapleID]->set_state(Button::State::PRESSED);
		}

		background = ColorBox(dimension.x(), dimension.y(), Color::Name::BLACK, 1.0f);

		Point<int16_t> textfield_pos = title_pos + Point<int16_t>(27, 69);

#pragma region Account
		Texture account_src = Texture(Title_new["mapleID"]);
		account_src_dim = account_src.get_dimensions();

		account = Textfield(Text::Font::A13M, Text::Alignment::LEFT, Color::Name::JAMBALAYA, Rectangle<int16_t>(textfield_pos, textfield_pos + account_src_dim), TEXTFIELD_LIMIT);

		account.set_key_callback
		(
			KeyAction::Id::TAB, [&]
			{
				account.set_state(Textfield::State::NORMAL);
				password.set_state(Textfield::State::FOCUSED);
			}
		);

		account.set_enter_callback
		(
			[&](std::string msg)
			{
				login();
			}
		);

		account_bg[false] = account_src;
		account_bg[true] = Title_new["nexonID"];
#pragma endregion

#pragma region Password
		textfield_pos.shift_y(account_src_dim.y() + 1);

		Texture password_src = Title_new["PW"];
		password_src_dim = password_src.get_dimensions();

		password = Textfield(Text::Font::A13M, Text::Alignment::LEFT, Color::Name::JAMBALAYA, Rectangle<int16_t>(textfield_pos, textfield_pos + password_src_dim), TEXTFIELD_LIMIT);

		password.set_key_callback
		(
			KeyAction::Id::TAB, [&]
			{
				account.set_state(Textfield::State::FOCUSED);
				password.set_state(Textfield::State::NORMAL);
			}
		);

		password.set_enter_callback
		(
			[&](std::string msg)
			{
				login();
			}
		);

		password.set_cryptchar('*');
		password_bg = password_src;
#pragma endregion

		saveid = Setting<SaveLogin>::get().load();

		if (saveid)
		{
			account.change_text(Setting<DefaultAccount>::get().load());
			password.set_state(Textfield::State::FOCUSED);
		}
		else
		{
			account.set_state(Textfield::State::FOCUSED);
		}

		if (Configuration::get().get_auto_login())
		{
			UI::get().emplace<UILoginWait>([]() {});

			auto loginwait = UI::get().get_element<UILoginWait>();

			if (loginwait && loginwait->is_active())
				LoginPacket(
					Configuration::get().get_auto_acc(),
					Configuration::get().get_auto_pass()
				).dispatch();
		}
	}

	void UILogin::draw(float alpha) const
	{
		background.draw(position + Point<int16_t>(0, 7));

		UIElement::draw(alpha);

		version.draw(position + version_pos - Point<int16_t>(0, 5));
		account.draw(position + Point<int16_t>(5, 10));
		password.draw(position + Point<int16_t>(5, 13));

		if (account.get_state() == Textfield::State::NORMAL && account.empty())
			account_bg[nexon].draw(position + title_pos);

		if (password.get_state() == Textfield::State::NORMAL && password.empty())
			password_bg.draw(position + title_pos);

		bool has_capslocks = UI::get().has_capslocks();

		check[saveid].draw(position + title_pos);

		if (has_capslocks && account.get_state() == Textfield::State::FOCUSED)
			capslock.draw(position + title_pos - Point<int16_t>(0, account_src_dim.y()));

		if (has_capslocks && password.get_state() == Textfield::State::FOCUSED)
			capslock.draw(position + title_pos + Point<int16_t>(password_src_dim.x() - account_src_dim.x(), 0));
	}

	void UILogin::update()
	{
		UIElement::update();

		account.update();
		password.update();
	}

	void UILogin::login()
	{
		account.set_state(Textfield::State::DISABLED);
		password.set_state(Textfield::State::DISABLED);

		std::string account_text = account.get_text();
		std::string password_text = password.get_text();

		std::function<void()> okhandler = [&, password_text]()
		{
			account.set_state(Textfield::State::NORMAL);
			password.set_state(Textfield::State::NORMAL);

			if (!password_text.empty())
				password.set_state(Textfield::State::FOCUSED);
			else
				account.set_state(Textfield::State::FOCUSED);
		};

		if (account_text.empty())
		{
			UI::get().emplace<UILoginNotice>(UILoginNotice::Message::NOT_REGISTERED, okhandler);
			return;
		}

		if (password_text.length() <= 4)
		{
			UI::get().emplace<UILoginNotice>(UILoginNotice::Message::WRONG_PASSWORD, okhandler);
			return;
		}

		UI::get().emplace<UILoginWait>(okhandler);

		auto loginwait = UI::get().get_element<UILoginWait>();

		if (loginwait && loginwait->is_active())
		{
			// TODO: Implement login with email
			if (nexon)
				LoginEmailPacket(account_text, password_text).dispatch();
			else
				LoginPacket(account_text, password_text).dispatch();
		}
	}

	void UILogin::open_url(uint16_t id)
	{
		std::string url;

		switch (id)
		{
			case Buttons::BtNew:
				url = Configuration::get().get_joinlink();
				break;
			case Buttons::BtHomePage:
				url = Configuration::get().get_website();
				break;
			case Buttons::BtPasswdLost:
				url = Configuration::get().get_findpass();
				break;
			case Buttons::BtEmailLost:
				url = Configuration::get().get_findid();
				break;
			default:
				return;
		}

		ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}

	Button::State UILogin::button_pressed(uint16_t id)
	{
		switch (id)
		{
			case Buttons::BtLogin:
			{
				login();

				return Button::State::NORMAL;
			}
			case Buttons::BtNew:
			case Buttons::BtHomePage:
			case Buttons::BtPasswdLost:
			case Buttons::BtEmailLost:
			{
				open_url(id);

				return Button::State::NORMAL;
			}
			case Buttons::BtEmailSave:
			{
				saveid = !saveid;

				Setting<SaveLogin>::get().save(saveid);

				return Button::State::MOUSEOVER;
			}
			case Buttons::BtQuit:
			{
				UI::get().quit();

				return Button::State::PRESSED;
			}
			case Buttons::BtMapleID:
			{
				nexon = false;

				buttons[Buttons::BtNexonID]->set_state(Button::State::NORMAL);

				account.change_text("");
				password.change_text("");

				account.set_limit(TEXTFIELD_LIMIT);

				return Button::State::PRESSED;
			}
			case Buttons::BtNexonID:
			{
				nexon = true;

				buttons[Buttons::BtMapleID]->set_state(Button::State::NORMAL);

				account.change_text("");
				password.change_text("");
				
				account.set_limit(72);

				return Button::State::PRESSED;
			}
			default:
			{
				return Button::State::DISABLED;
			}
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

	UIElement::Type UILogin::get_type() const
	{
		return TYPE;
	}
}