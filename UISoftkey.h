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
#include "UIElement.h"
#include "Textlabel.h"
#include "Randomizer.h"
#include "UI.h"
#include "Login.h"
#include "Session.h"

using namespace Net;

namespace IO
{
	enum SoftkeyButtons
	{
		BT_SOFTKEY_0,
		BT_SOFTKEY_1,
		BT_SOFTKEY_2,
		BT_SOFTKEY_3,
		BT_SOFTKEY_4,
		BT_SOFTKEY_5,
		BT_SOFTKEY_6,
		BT_SOFTKEY_7,
		BT_SOFTKEY_8,
		BT_SOFTKEY_9,
		BT_SOFTKEY_NEXT,
		BT_SOFTKEY_BACK,
		BT_SOFTKEY_CANCEL,
		BT_SOFTKEY_OK
	};

	enum SoftkeyType
	{
		SFTK_REGISTER,
		SFTK_CHARSELECT,
		SFTK_CHARDEL,
		SFTK_MERCHANT
	};

	class UISoftkey : public UIElement
	{
	public:
		UISoftkey(SoftkeyType, UI&, Login&, Session&);
		~UISoftkey();
		void draw();
		void buttonpressed(short);
		void shufflekeys();
		vector2d<int> keypos(char);
	private:
		SoftkeyType type;
		UI& ui;
		Login& login;
		Session& session;
		Textlabel* entry;
		Randomizer random;
	};

	class ElementSoftkey : public Element
	{
	public:
		ElementSoftkey(SoftkeyType t, UI& u, Login& lg, Session& ses) : ui(u), login(lg), session(ses)
		{
			sktype = t;
		}

		bool isfocused() const
		{
			return true;
		}

		UIType type() const
		{
			return UI_SOFTKEYBOARD;
		}

		UISoftkey* instantiate() const
		{
			return new UISoftkey(sktype, ui, login, session);
		}
	private:
		SoftkeyType sktype;
		UI& ui;
		Login& login;
		Session& session;
	};
}

