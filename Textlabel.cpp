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
#include "Textlabel.h"
#include "TextWrapperDW.h"

namespace Graphics
{
	Textlabel::Textlabel(Font f, Textcolor c, string s, short m)
	{
		text = new TextWrapperDW(f, c, s, m);
		str = s;
	}

	Textlabel::~Textlabel()
	{
		delete text;
	}

	void Textlabel::draw(vector2d<int> pos)
	{
		if (text != 0)
		{
			text->draw(pos);
		}
	}

	void Textlabel::settext(string t)
	{
		settext(t, 816);
	}

	void Textlabel::settext(string t, short m)
	{
		str = t;
		if (text != 0)
		{
			text->settext(t, m);
		}
	}

	short Textlabel::getadvance(size_t pos)
	{
		return (text != 0) ? text->getadvance(pos) : 0;
	}

	size_t Textlabel::getlength()
	{
		return str.size();
	}

	string Textlabel::gettext()
	{
		return str;
	}
}
