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
#include "KeyTarget.h"
#include "Element.h"
#include "Cursor.h"
#include "Ptrmap.h"

namespace IO
{
	using::Util::Ptrmap;

	class UI : public KeyTarget
	{
	public:
		UI();
		~UI();
		void init();
		void draw() const;
		void update(uint16_t);
		void enable();
		void disable();
		void sendmouse(vector2d<int32_t>);
		void sendmouse(Mousestate, vector2d<int32_t>);
		void sendkey(Keytype, int32_t, bool);
		void add(const Element&);
		void remove(UIType);
		UIElement& getelement(UIType) const;
	private:
		Ptrmap<UIType, UIElement> elements;
		UINull nullelement;
		UIType focused;
		Cursor cursor;
		bool enabled;
	};
}