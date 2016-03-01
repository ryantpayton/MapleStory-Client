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
#include "UIElement.h"
#include "Keyboard.h"
#include "Console.h"

#include "Components\Icon.h"

namespace IO
{
	class UIState
	{
	public:
		virtual ~UIState() {}

		virtual void draw(float inter, Point<int16_t> cursor) const = 0;
		virtual void update() = 0;
		virtual void doubleclick(Point<int16_t> pos) = 0;
		virtual void dragicon(Icon* icon) = 0;
		virtual void sendkey(Keyboard::Keytype type, int32_t action, bool pressed) = 0;
		virtual Cursor::State sendmouse(Cursor::State mst, Point<int16_t> pos) = 0;

		virtual void add(const Element& element) = 0;
		virtual void remove(UIElement::Type type) = 0;
		virtual UIElement* get(UIElement::Type type) const = 0;
		virtual UIElement* getfront(Point<int16_t> pos) const = 0;
	};

	class UIStateNull : public UIState
	{
		void draw(float, Point<int16_t>) const override { Console::get().print("Warning: UI was not initialized."); }
		void update() override {}
		void doubleclick(Point<int16_t>) override {}
		void dragicon(Icon*) override {}
		void sendkey(Keyboard::Keytype, int32_t, bool) override {}
		Cursor::State sendmouse(Cursor::State, Point<int16_t>) override { return Cursor::IDLE; }
		void add(const Element&) override {}
		void remove(UIElement::Type) override {}
		UIElement* get(UIElement::Type) const override { return nullptr; }
		UIElement* getfront(Point<int16_t>) const override { return nullptr; }
	};
}