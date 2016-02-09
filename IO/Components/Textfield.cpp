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
#include "Constants.h"

namespace IO
{
	Textfield::Textfield(Text::Font font, Text::Alignment alignment, 
		Text::Color color, rectangle2d<int16_t> bnd, size_t lim) {

		textlabel = Text(font, alignment, color);
		marker = Text(font, alignment, color);
		marker.settext("|");
		bounds = bnd;
		limit = lim;
		text = "";
		markerpos = 0;
		crypt = 0;
		state = NORMAL;
		onreturn = nullptr;
	}

	Textfield::Textfield()
	{
		text = "";
		onreturn = nullptr;
	}

	Textfield::~Textfield() {}

	void Textfield::draw(vector2d<int16_t> parentpos) const
	{
		if (state == DISABLED)
			return;
		
		vector2d<int16_t> absp = bounds.getlt() + parentpos;
		if (text.size() > 0)
		{
			textlabel.draw(absp);
		}

		if (state == FOCUSED && showmarker)
		{
			vector2d<int16_t> mpos = absp + vector2d<int16_t>(textlabel.advance(markerpos), -1);
			marker.draw(mpos);
		}
	}

	void Textfield::update()
	{
		if (state == DISABLED)
			return;

		elapsed += Constants::TIMESTEP;
		if (elapsed > 256)
		{
			showmarker = !showmarker;
			elapsed = 0;
		}
	}

	void Textfield::setstate(State st)
	{
		if (state != st)
		{
			state = st;
			elapsed = 0;
			showmarker = true;
		}
	}

	void Textfield::setonreturn(void(*or)(string))
	{
		onreturn = or;
	}

	void Textfield::sendkey(Keyboard::Keytype type, int32_t key, bool pressed)
	{
		switch (type)
		{
		case Keyboard::ACTION:
			if (pressed)
			{
				switch (key)
				{
				case Keyboard::LEFT:
					if (markerpos > 0)
					{
						markerpos--;
					}
					break;
				case Keyboard::RIGHT:
					if (markerpos < text.size())
					{
						markerpos++;
					}
					break;
				case Keyboard::BACK:
					if (text.size() > 0 && markerpos > 0)
					{
						text.erase(markerpos - 1, 1);
						markerpos--;
						modifytext(text);
					}
					break;
				case Keyboard::RETURN:
					if (onreturn)
					{
						(*onreturn)(text);
						text = "";
						markerpos = 0;
						modifytext(text);
					}
					break;
				case Keyboard::SPACE:
					if (belowlimit())
					{
						text.insert(markerpos, 1, ' ');
						markerpos++;
						modifytext(text);
					}
					break;
				}
			}
			break;
		case Keyboard::LETTER:
		case Keyboard::NUMBER:
			if (!pressed)
			{
				int8_t c = static_cast<int8_t>(key);
				if (belowlimit())
				{
					text.insert(markerpos, 1, c);
					markerpos++;
					modifytext(text);
				}
			}
			break;
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

	bool Textfield::belowlimit() const
	{
		if (limit > 0)
		{
			return text.size() < limit;
		}
		else
		{
			uint16_t advance = textlabel.advance(text.size());
			return (advance + 50) < bounds.gethor().length();
		}
	}

	string Textfield::gettext() const
	{
		return text;
	}

	Textfield::State Textfield::getstate() const
	{
		return state;
	}

	rectangle2d<int16_t> Textfield::getbounds(vector2d<int16_t> parentpos) const
	{
		return rectangle2d<int16_t>(
			bounds.getlt() + parentpos,
			bounds.getrb() + parentpos
			);
	}
}
