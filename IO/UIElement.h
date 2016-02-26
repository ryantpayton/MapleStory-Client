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
#include "Graphics\Sprite.h"
#include "Components\Button.h"
#include <memory>

namespace IO
{
	using std::vector;
	using std::unique_ptr;
	using Graphics::Sprite;

	// Base class for all types of user interfaces on screen.
	class UIElement
	{
	public:
		enum Type
		{
			NONE,
			LOGIN,
			LOGINWAIT,
			LOGINNOTICE,
			WORLDSELECT,
			CHARSELECT,
			CHARCREATION,
			SOFTKEYBOARD,
			STATUSMESSENGER,
			STATUSBAR,
			BUFFLIST,
			NPCTALK,
			STATSINFO,
			ITEMINVENTORY,
			EQUIPINVENTORY,
		};

		virtual ~UIElement();

		virtual void draw(float inter) const;
		virtual void update();

		void makeactive();
		void deactivate();
		bool isactive() const;

		virtual void togglehide();
		virtual void buttonpressed(uint16_t buttonid);
		virtual void doubleclick(vector2d<int16_t> cursorpos);
		virtual void icondropped(int16_t identifier);
		virtual void dropicon(vector2d<int16_t> cursorpos, Type type, int16_t identifier);
		virtual Cursor::State sendmouse(bool clicked, vector2d<int16_t> cursorpos);
		virtual rectangle2d<int16_t> bounds() const;

	protected:
		UIElement();

		vector2d<int16_t> dimension;
		map<uint16_t, unique_ptr<Button>> buttons;
		vector<Sprite> sprites;

		vector2d<int16_t> position;
		bool active;
	};
}