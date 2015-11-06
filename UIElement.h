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
#include "Button.h"
#include "Graphic.h"
#include "Cursor.h"
#include "Ptrvector.h"

namespace IO
{
	using::Util::Ptrvector;
	using::Graphics::Graphic;

	class UIElement
	{
	public:
		virtual ~UIElement() {}
		virtual void togglehide() { active = !active; }
		virtual void makeactive() { active = true; }
		virtual void deactivate() { active = false; }
		virtual void draw() const;
		virtual void update(uint16_t);
		virtual void buttonpressed(uint16_t) = 0;
		virtual Mousestate sendmouse(bool, vector2d<int32_t>);
		virtual rectangle2d<int32_t> bounds() const;
		bool isactive() const { return active; }
	protected:
		bool active;
		vector2d<int32_t> position;
		vector2d<int32_t> dimension;
		Ptrmap<uint16_t, Button> buttons;
		Ptrvector<Graphic> graphics;
	};

	class UINull : public UIElement
	{
	public:
		void draw(){}
		void update(uint16_t){}
		void buttonpressed(uint16_t){}
		Mousestate sendmouse(bool, vector2d<int32_t>){ return MST_IDLE; }
		rectangle2d<int32_t> bounds() { return rectangle2d<int32_t>(); }
	};
}