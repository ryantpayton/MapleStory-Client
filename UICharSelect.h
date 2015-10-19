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
#include "Equipcache.h"
#include "UI.h"
#include "Login.h"
#include "Session.h"
#include "Nametag.h"
#include "Charset.h"

using namespace Data;
using namespace Net;

namespace IO
{
	enum CharSelectButtons
	{
		BT_CREATECHAR,
		BT_DELETECHAR,
		BT_SELECTCHAR,
		BT_ARBEIT,
		BT_CARDS,
		BT_PAGELEFT,
		BT_PAGERIGHT,
		BT_CHAR0
	};

	class UICharSelect : public UIElement
	{
	public:
		UICharSelect(Equipcache&, UI&, Login&, Session&);
		~UICharSelect();
		void draw();
		void update(short);
		void buttonpressed(short);
		void addchar(char);
		void removechar(char);
	private:
		void selectchar();
		vector2d<int> getcharpos(size_t);
		Equipcache& equips;
		UI& ui;
		Login& login;
		Session& session;
		vector<CharLook> charlooks;
		char charcount;
		char selected;
		char page;
		Textlabel* namelabel;
		Textlabel* joblabel;
		Ptrvector<Nametag> nametags;
		Charset* levelset;
		Charset* statsset;
	};

	class ElementCharSelect : public Element
	{
	public:
		ElementCharSelect(Equipcache& eq, UI& u, Login& lg, Session& ses) : equips(eq), ui(u), login(lg), session(ses) {}

		UIType type() const
		{
			return UI_CHARSELECT;
		}

		UIElement* instantiate() const
		{
			return new UICharSelect(equips, ui, login, session);
		}
	private:
		Equipcache& equips;
		UI& ui;
		Login& login;
		Session& session;
	};
}

