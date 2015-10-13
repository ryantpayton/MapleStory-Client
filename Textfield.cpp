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
#include "Textfield.h"

namespace IO
{
	Textfield::Textfield(Font fnt, Textcolor col, vector2d<int> pos, size_t lim)
	{
		text = new Textlabel(fnt, col, "", 0);
		marker = new Textlabel(fnt, col, "|", 0);
		str = "";
		position = pos;
		limit = lim;
		cryptchar = 0;
		state = TXFS_NORMAL;
	}

	Textfield::~Textfield()
	{
		delete text;
		delete marker;
	}

	void Textfield::draw(vector2d<int> parentpos)
	{
		if (state != TXFS_DISABLED)
		{
			vector2d<int> absp = position + parentpos;
			if (str.size() > 0)
			{
				text->draw(absp);
			}
			if (state == TXFS_FOCUSED && showmarker)
			{
				vector2d<int> mpos = absp + vector2d<int>(text->getadvance(markerpos), 0);
				marker->draw(mpos);
			}
		}
	}

	void Textfield::update(short dpf)
	{
		elapsed += dpf;
		if (elapsed > 256)
		{
			showmarker = !showmarker;
			elapsed = 0;
		}
	}

	void Textfield::setstate(TextfieldState st)
	{
		if (state != st)
		{
			state = st;
			elapsed = 0;
			showmarker = true;
		}
	}

	void Textfield::sendinput(char c)
	{
		switch (c)
		{
		case 0:
			if (str.size() > 0)
			{
				str.erase(markerpos - 1, 1);
				settext(str);
				markerpos--;
			}
			break;
		case 1:
			if (markerpos > 0)
			{
				markerpos--;
			}
			break;
		case 2:
			if (markerpos < str.size())
			{
				markerpos++;
			}
			break;
		default:
			if (str.size() < limit)
			{
				str.insert(markerpos, 1, c);
				settext(str);
				markerpos++;
			}
		}
	}

	void Textfield::settext(string t)
	{
		if (cryptchar > 0)
		{
			string crypted;
			crypted.insert(0, t.size(), cryptchar);
			text->settext(crypted);
		}
		else
		{
			text->settext(t);
		}
	}

	rectangle2d<int> Textfield::bounds(vector2d<int> parentpos)
	{
		vector2d<int> absp = position + parentpos;
		return rectangle2d<int>(absp, absp + vector2d<int>(14 * (int)limit, 24));
	}
}
