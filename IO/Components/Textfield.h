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
#include "..\Cursor.h"
#include "..\Keyboard.h"

#include "..\..\Graphics\Text.h"
#include "..\..\Util\Rectangle.h"

#include <map>
#include <functional>

namespace jrc
{
	class Textfield
	{
	public:
		enum State
		{
			NORMAL,
			DISABLED,
			FOCUSED
		};

		Textfield(Text::Font font, Text::Alignment alignment, Text::Color color,
			Rectangle<int16_t> bounds, size_t limit);
		Textfield();
		~Textfield();

		void draw(Point<int16_t> position) const;
		void update(Point<int16_t> parentpos);
		void sendkey(Keyboard::Keytype type, int32_t code, bool down);
		void sendstring(std::string str);

		void setstate(State state);
		void settext(std::string text);
		void setcrypt(int8_t character);

		void setonreturn(std::function<void(std::string)> onreturn);
		void setkey(Keyboard::Action key, std::function<void(void)> action);

		Cursor::State sendcursor(Point<int16_t> cursorpos, bool clicked);

		std::string gettext() const;
		State getstate() const;
		Rectangle<int16_t> getbounds() const;

	private:
		void modifytext(std::string);
		bool belowlimit() const;

		Text textlabel;
		std::string text;
		Text marker;
		bool showmarker;
		uint16_t elapsed;
		size_t markerpos;
		Rectangle<int16_t> bounds;
		Point<int16_t> parentpos;
		size_t limit;
		int8_t crypt;
		State state;

		std::map<int32_t, std::function<void(void)>> callbacks;
		std::function<void(std::string)> onreturn;
	};
}

