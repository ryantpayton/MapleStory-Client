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
#include "UIElement.h"
#include "UI.h"
#include "Login.h"
#include "Session.h"

namespace IO
{
	enum WorldSelectButtons
	{
		BT_ENTERWORLD = 0,
		BT_WORLD0 = 1,
		BT_CHANNEL0 = 17
	};

	using::Net::Login;
	using::Net::Session;

	class UIWorldSelect : public UIElement
	{
	public:
		UIWorldSelect(UI&, Login&, Session&);
		void buttonpressed(uint16_t);
	private:
		UI& ui;
		Login& login;
		Session& session;
		uint8_t worldid;
		uint8_t channelid;
	};

	class ElementWorldSelect : public Element
	{
	public:
		ElementWorldSelect(UI& u, Login& lg, Session& ses) : ui(u), login(lg), session(ses) {}

		UIType type() const
		{
			return UI_WORLDSELECT;
		}

		UIElement* instantiate() const
		{
			return new UIWorldSelect(ui, login, session);
		}
	private:
		UI& ui;
		Login& login;
		Session& session;
	};
}

