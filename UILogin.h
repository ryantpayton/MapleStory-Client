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
#include "Element.h"
#include "Window.h"
#include "Session.h"
#include "UI.h"
#include "Configuration.h"
#include "Textfield.h"

namespace IO
{
	enum UILoginButtons
	{
		BT_LOGIN,
		BT_REGISTER,
		BT_HOMEPAGE,
		BT_PASSLOST,
		BT_IDLOST,
		BT_SAVEID,
		BT_QUIT
	};

	using::Net::Session;
	using::Util::Configuration;
	using::Graphics::Texture;

	class UILogin : public UIElement
	{
	public:
		UILogin(Window&, Session&, UI&, Configuration&);
		~UILogin();
		void draw() const;
		void update(uint16_t);
		void buttonpressed(uint16_t);
		Mousestate sendmouse(bool, vector2d<int32_t>);
	private:
		Window& window;
		Session& session;
		UI& ui;
		Configuration& config;
		Textfield account;
		Textfield password;
		Texture* accountbg;
		Texture* passwordbg;
		Ptrmap<bool, Texture> checkbox;
		bool saveid;
	};

	class ElementLogin : public Element
	{
	public:
		ElementLogin(Window& wn, Session& ses, UI& u, Configuration& cfg) : window(wn), session(ses), ui(u), config(cfg){}

		UIType type() const
		{
			return UI_LOGIN;
		}

		UIElement* instantiate() const
		{
			return new UILogin(window, session, ui, config);
		}
	private:
		Window& window;
		Session& session;
		UI& ui;
		Configuration& config;
	};
}