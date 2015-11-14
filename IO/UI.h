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
#include "Program\ClientInterface.h"
#include "IO\KeyTarget.h"
#include "IO\Element.h"
#include "IO\Cursor.h"
#include "IO\Keyboard.h"
#include <unordered_map>
#include <memory>

namespace IO
{
	using::std::unique_ptr;
	using::std::unordered_map;
	using Journey::ClientInterface;

	// Class that manages all the different UIs.
	class UI : public UIInterface
	{
	public:
		UI(ClientInterface&);

		void init();
		void draw(float) const;
		void update();
		void enable() override;
		void disable() override;
		void sendmouse(vector2d<int32_t>);
		void sendmouse(Mousestate, vector2d<int32_t>);
		void sendkey(Keytype, int32_t, bool) override;
		void add(const Element&) override;
		void remove(Element::UIType) override;
		UIElement* getelement(Element::UIType) const override;
		Keyboard& getkeyboard() override;

	private:
		unordered_map<Element::UIType, unique_ptr<UIElement>> elements;
		Element::UIType focused;
		Cursor cursor;
		Keyboard keyboard;
		bool enabled;

		ClientInterface& client;
	};
}