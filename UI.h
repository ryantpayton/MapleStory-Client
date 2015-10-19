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
#include "Element.h"
#include "Cursor.h"
#include "InputHandler.h"
#include "Ptrmap.h"

using namespace Util;

namespace IO
{
	class UI
	{
	public:
		UI();
		~UI();
		void init();
		void draw();
		void update(short);
		void sendmouse(vector2d<int>);
		void sendmouse(Mousestate, vector2d<int>);
		void add(const Element&);
		void remove(UIType);
		UIElement* getelement(UIType);
		InputHandler* gethandler() { return handler; }
		void enable() { enabled = true; }
		void disable() { enabled = false; }
	private:
		Ptrmap<UIType, UIElement> elements;
		Cursor cursor;
		InputHandler* handler;
		bool enabled;
	};
}