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
#include "Element.h"
#include "Session.h"
#include "UI.h"
#include "Textfield.h"

using namespace Net;

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

	class UILogin : public UIElement
	{
	public:
		UILogin(Session*, UI*);
		~UILogin();
		void draw();
		void update(short);
		void buttonpressed(short);
		Mousestate sendmouse(bool, vector2d<int>);
	private:
		Session* session;
		UI* ui;
		Textfield* account;
		Textfield* password;
		Texture* accountbg;
		Texture* passwordbg;
		ptrmap<bool, Texture*> checkbox;
		bool saveid;
	};

	class ElementLogin : public Element
	{
	public:
		ElementLogin(Session* ses, UI* u) 
		{
			session = ses;
			ui = u;
		}

		UIType type()
		{
			return UI_LOGIN;
		}

		UIElement* instantiate()
		{
			return new UILogin(session, ui);
		}
	private:
		Session* session;
		UI* ui;
	};
}