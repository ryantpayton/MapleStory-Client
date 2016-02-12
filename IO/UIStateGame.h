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
#include "UIState.h"
#include <unordered_map>
#include <list>
#include <memory>

namespace IO
{
	using std::unordered_map;
	using std::list;
	using std::unique_ptr;

	class UIStateGame : public UIState
	{
	public:
		UIStateGame();
		
		void draw(float inter, vector2d<int16_t> cursor) const override;
		void update() override;
		void doubleclick(vector2d<int16_t> pos) override;
		void dragicon(Icon* icon) override;
		void sendkey(Keyboard::Keytype type, int32_t action, bool pressed) override;
		Cursor::State sendmouse(Cursor::State mst, vector2d<int16_t> pos) override;

		void add(const Element& element) override;
		void remove(Element::UIType type) override;
		UIElement* get(Element::UIType type) const override;
		UIElement* getfront(vector2d<int16_t> pos) const override;

	private:
		void dropicon(vector2d<int16_t> pos, Element::UIType parent, int16_t identifier);

		unordered_map<Element::UIType, unique_ptr<UIElement>> elements;
		list<Element::UIType> elementorder;
		Element::UIType focused;

		Icon* draggedicon;
	};
}