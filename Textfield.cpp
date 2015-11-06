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
#include "Textfield.h"

namespace IO
{
	Textfield::Textfield(Font fnt, Textcolor col, vector2d<int> pos, size_t lim)
	{
		textlabel = Textlabel(fnt, col, "", 0);
		marker = Textlabel(fnt, col, "|", 0);
		text = "";
		markerpos = 0;
		position = pos;
		limit = lim;
		crypt = 0;
		state = TXFS_NORMAL;
	}

	void Textfield::draw(vector2d<int32_t> parentpos) const
	{
		if (state != TXFS_DISABLED)
		{
			vector2d<int32_t> absp = position + parentpos;
			if (text.size() > 0)
			{
				textlabel.gettext().draw(absp);
			}
			if (state == TXFS_FOCUSED && showmarker)
			{
				vector2d<int32_t> mpos = absp + vector2d<int32_t>(textlabel.gettext().getadvance(markerpos), 0);
				marker.gettext().draw(mpos);
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

	void Textfield::sendkey(Keytype type, int key, bool down)
	{
		if (type == KT_ACTION)
		{
			switch (key)
			{
			case KA_LEFT:
				if (markerpos > 0)
				{
					markerpos--;
				}
				break;
			case KA_RIGHT:
				if (markerpos < text.size())
				{
					markerpos++;
				}
				break;
			case KA_BACK:
				if (text.size() > 0)
				{
					text.erase(markerpos - 1, 1);
					settext(text);
					markerpos--;
				}
				break;
			}
		}
		else if (type == KT_LETTER)
		{
			char c = static_cast<char>(key);
			if (text.size() < limit)
			{
				text.insert(markerpos, 1, c);
				settext(text);
				markerpos++;
			}
		}
	}

	void Textfield::settext(string t)
	{
		if (crypt > 0)
		{
			string crypted;
			crypted.insert(0, t.size(), crypt);
			textlabel.gettext().settext(crypted, 0);
		}
		else
		{
			textlabel.gettext().settext(t, 0);
		}
		text = t;
	}

	void Textfield::setcrypt(char c)
	{
		crypt = c;
	}

	string Textfield::gettext() const
	{
		return text;
	}

	TextfieldState Textfield::getstate() const
	{
		return state;
	}

	rectangle2d<int> Textfield::bounds(vector2d<int> parentpos) const
	{
		vector2d<int> absp = position + parentpos;
		return rectangle2d<int>(absp, absp + vector2d<int>(14 * (int)limit, 24));
	}
}
