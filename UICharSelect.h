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
#include "Equipcache.h"
#include "UI.h"
#include "Login.h"
#include "Session.h"
#include "Nametag.h"
#include "Charset.h"

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

	using::std::vector;
	using::Data::Equipcache;
	using::Net::Login;
	using::Net::Session;
	using::Character::CharLook;

	// The character selection screen.
	class UICharSelect : public UIElement
	{
	public:
		UICharSelect(Equipcache&, UI&, Login&, Session&);
		~UICharSelect();
		void draw() const;
		void update(uint16_t);
		void buttonpressed(uint16_t);
		void addchar(uint8_t);
		void removechar(uint8_t);
	private:
		void selectchar();
		vector2d<int32_t> getcharpos(size_t) const;
		Equipcache& equips;
		UI& ui;
		Login& login;
		Session& session;
		vector<CharLook> charlooks;
		uint8_t charcount;
		uint8_t selected;
		uint8_t page;
		Textlabel namelabel;
		Textlabel joblabel;
		Ptrvector<Nametag> nametags;
		Charset* levelset;
		Charset* statsset;
	};

	// Factory for the character selection screen.
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

