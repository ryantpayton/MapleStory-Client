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
#include "Graphics\Textlabel.h"
#include "IO\Keyboard.h"
#include "Util\FunctionPointers.h"
#include "Util\rectangle2d.h"

namespace IO
{
	using std::string;
	using Util::vector2d;
	using Util::rectangle2d;
	using Util::Consumer;
	using Graphics::Textlabel;

	class Textfield
	{
	public:
		enum State
		{
			NORMAL,
			DISABLED,
			FOCUSED
		};

		Textfield(Textlabel::Font font, Textlabel::Textcolor color, 
			rectangle2d<int16_t> bounds, size_t limit);
		Textfield();
		~Textfield();

		void draw(vector2d<int16_t> position) const;
		void update();
		void sendkey(Keyboard::Keytype type, int32_t code, bool down);

		void setstate(State state);
		void settext(string text);
		void setcrypt(int8_t character);
		void setonreturn(Consumer<string> onreturn);

		string gettext() const;
		State getstate() const;
		rectangle2d<int16_t> getbounds(vector2d<int16_t> parentpos) const;

	private:
		void modifytext(string);
		bool belowlimit() const;

		Textlabel textlabel;
		string text;
		Textlabel marker;
		bool showmarker;
		uint16_t elapsed;
		size_t markerpos;
		rectangle2d<int16_t> bounds;
		size_t limit;
		int8_t crypt;
		State state;
		Consumer<string> onreturn;
	};
}

