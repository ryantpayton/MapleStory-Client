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
#include "Cursor.h"
#include "Keyboard.h"
#include "Components\StatusMessenger.h"
#include "Components\Textfield.h"
#include "Components\Icon.h"
#include "Util\Singleton.h"
#include <unordered_map>
#include <list>
#include <memory>

namespace IO
{
	using std::unique_ptr;
	using std::unordered_map;
	using std::list;

	class UI : public Singleton<UI>
	{
	public:
		enum Mode
		{
			MD_LOGIN,
			MD_GAME
		};

		UI();

		void draw(float inter) const;
		void update();

		void sendmouse(vector2d<int16_t> pos);
		void sendmouse(bool pressed, vector2d<int16_t> pos);
		void doubleclick(vector2d<int16_t> pos);
		void sendkey(int32_t keycode, bool pressed);

		void shownpctalk(int32_t npcid, int8_t type, int16_t style, int8_t speaker, string text);
		void showstatus(Text::Color color, string message);
		void focustextfield(Textfield*);
		void dragicon(Icon*);

		void addkeymapping(uint8_t no, uint8_t type, int32_t action);
		void enable();
		void disable();
		void changemode(Mode mode);

		void add(const Element& type);
		void remove(Element::UIType type);

		bool haselement(Element::UIType type) const;
		UIElement* getelement(Element::UIType type) const;

		template <class T>
		T* getelement(Element::UIType type) const
		{
			UIElement* element = getelement(type);
			if (element)
				return reinterpret_cast<T*>(element);
			else
				return nullptr;
		}

	private:
		void addelementbykey(int32_t key);
		void dropicon(vector2d<int16_t> pos);
		void sendmouse(Cursor::Mousestate mst, vector2d<int16_t> pos);
		UIElement* getfront(vector2d<int16_t> pos);

		Keyboard keyboard;
		Cursor cursor;
		StatusMessenger messenger;

		unordered_map<Element::UIType, unique_ptr<UIElement>> elements;
		list<Element::UIType> elementorder;
		unordered_map<int32_t, bool> keydown;

		Textfield* focusedtextfield;
		Icon* draggedicon;

		Element::UIType focused;
		Mode mode;
		bool enabled;
	};
}