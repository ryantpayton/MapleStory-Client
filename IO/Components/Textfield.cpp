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
#include "Program\Constants.h"

namespace IO
{
	Textfield::Textfield(Textlabel::Font fnt, Textlabel::Textcolor col, vector2d<int32_t> pos, size_t lim)
	{
		textlabel = Textlabel(fnt, col, "", 0);
		marker = Textlabel(fnt, col, "|", 0);
		text = "";
		markerpos = 0;
		position = pos;
		limit = lim;
		crypt = 0;
		state = NORMAL;
	}

	Textfield::Textfield()
	{
		text = "";
	}

	void Textfield::draw(vector2d<int32_t> parentpos) const
	{
		if (state != DISABLED)
		{
			vector2d<int32_t> absp = position + parentpos;
			if (text.size() > 0)
			{
				textlabel.draw(absp);
			}
			if (state == FOCUSED && showmarker)
			{
				vector2d<int32_t> mpos = absp + vector2d<int32_t>(textlabel.getadvance(markerpos), 0);
				marker.draw(mpos);
			}
		}
	}

	void Textfield::update()
	{
		elapsed += Constants::TIMESTEP;
		if (elapsed > 256)
		{
			showmarker = !showmarker;
			elapsed = 0;
		}
	}

	void Textfield::setstate(TfState st)
	{
		if (state != st)
		{
			state = st;
			elapsed = 0;
			showmarker = true;
		}
	}

	void Textfield::sendkey(Keytype type, int32_t key, bool down)
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
				if (text.size() > 0 && markerpos > 0)
				{
					text.erase(markerpos - 1, 1);
					markerpos--;
					modifytext(text);
				}
				break;
			}
		}
		else if (type == KT_LETTER)
		{
			int8_t c = static_cast<int8_t>(key);
			if (text.size() < limit)
			{
				text.insert(markerpos, 1, c);
				markerpos++;
				modifytext(text);
			}
		}
	}

	void Textfield::modifytext(string t)
	{
		if (crypt > 0)
		{
			string crypted;
			crypted.insert(0, t.size(), crypt);
			textlabel.settext(crypted, 0);
		}
		else
		{
			textlabel.settext(t, 0);
		}

		text = t;
	}

	void Textfield::settext(string t)
	{
		modifytext(t);
		markerpos = text.size();
	}

	void Textfield::setcrypt(int8_t c)
	{
		crypt = c;
	}

	string Textfield::gettext() const
	{
		return text;
	}

	Textfield::TfState Textfield::getstate() const
	{
		return state;
	}

	rectangle2d<int32_t> Textfield::bounds(vector2d<int32_t> parentpos) const
	{
		vector2d<int32_t> absp = position + parentpos;
		return rectangle2d<int32_t>(absp, absp + vector2d<int32_t>(14 * (int32_t)limit, 24));
	}
}
