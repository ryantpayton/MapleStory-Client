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
#include "Cursor.h"
#include "Keyboard.h"
#include "Element.h"
#include "UIState.h"

#include "Components\Icon.h"
#include "Components\Textfield.h"

#include "Util\Singleton.h"

#include <unordered_map>

namespace IO
{
	using std::unique_ptr;
	using std::unordered_map;

	class UI : public Singleton<UI>
	{
	public:
		enum State
		{
			LOGIN,
			GAME
		};

		UI();

		void init();
		void draw(float inter) const;
		void update();

		void sendmouse(vector2d<int16_t> pos);
		void sendmouse(bool pressed, vector2d<int16_t> pos);
		void sendmouse(bool pressed);
		void doubleclick(vector2d<int16_t> pos);
		void doubleclick();
		void sendkey(int32_t keycode, bool pressed);
		void focustextfield(Textfield* textfield);
		void dragicon(Icon* icon);

		void addkeymapping(uint8_t no, uint8_t type, int32_t action);
		void enable();
		void disable();
		void changestate(State mode);

		void add(const Element& type);
		void remove(UIElement::Type type);

		bool haselement(UIElement::Type type) const;
		UIElement* getelement(UIElement::Type type) const;

		template <class T>
		T* getelement(UIElement::Type type) const;

	private:
		unique_ptr<UIState> state;
		Keyboard keyboard;
		Cursor cursor;

		unordered_map<int32_t, bool> keydown;

		Textfield* focusedtextfield;
		bool enabled;

		int16_t clickrepeat;
	};

	template <class T>
	T* UI::getelement(UIElement::Type type) const
	{
		UIElement* element = getelement(type);
		return element ? reinterpret_cast<T*>(element) : nullptr;
	}
}