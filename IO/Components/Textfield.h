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
#include "Util\rectangle2d.h"
#include "IO\InputConstants.h"

namespace IO
{
	using::std::string;
	using::Util::vector2d;
	using::Util::rectangle2d;
	using::Graphics::Textlabel;

	class Textfield
	{
	public:
		enum TfState
		{
			NORMAL,
			DISABLED,
			FOCUSED
		};

		Textfield(Textlabel::Font, Textlabel::Textcolor, vector2d<int16_t>, size_t);
		Textfield();
		void draw(vector2d<int16_t>) const;
		void update();
		void sendkey(Keytype, int32_t, bool);
		void setstate(TfState);
		void settext(string);
		void setcrypt(int8_t);
		string gettext() const;
		TfState getstate() const;
		rectangle2d<int16_t> bounds(vector2d<int16_t>) const;

	private:
		void modifytext(string);

		Textlabel textlabel;
		string text;
		Textlabel marker;
		bool showmarker;
		uint16_t elapsed;
		size_t markerpos;
		vector2d<int16_t> position;
		size_t limit;
		int8_t crypt;
		TfState state;
	};
}

