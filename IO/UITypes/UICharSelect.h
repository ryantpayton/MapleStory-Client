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
#include "IO\Element.h"
#include "IO\Components\Nametag.h"
#include "IO\Components\Charset.h"
#include "Character\Look\CharLook.h"

namespace IO
{
	using std::vector;
	using Character::CharLook;
	using Character::Maplestat;

	// The character selection screen.
	class UICharSelect : public UIElement
	{
	public:
		enum Buttons
		{
			BT_CREATECHAR,
			BT_DELETECHAR,
			BT_SELECTCHAR,
			BT_ARBEIT,
			BT_CARDS,
			BT_PAGELEFT,
			BT_PAGERIGHT,
			BT_CHAR0
		};

		UICharSelect();
		void draw(float) const override;
		void update() override;
		void buttonpressed(uint16_t) override;
		void removechar(uint8_t id);

	private:
		void addchar(uint8_t id);
		void selectchar();
		void updateinfo();
		string getstringfor(size_t label) const;
		Point<int16_t> getlabelpos(size_t label) const;
		Point<int16_t> getcharpos(size_t id) const;

		Point<int16_t> selworldpos;
		Point<int16_t> charinfopos;

		bool haschars;
		vector<CharLook> charlooks;
		uint8_t charcount;
		uint8_t charslots;
		uint8_t selected;
		uint8_t page;
		vector<Nametag> nametags;
		Charset levelset;

		struct OutlinedText
		{
			Text inner;
			Text l;
			Text r;
			Text t;
			Text b;

			OutlinedText(Text::Font font, Text::Alignment alignment)
			{
				inner = Text(font, alignment, Text::WHITE);
				l = Text(font, alignment, Text::DARKGREY);
				r = Text(font, alignment, Text::DARKGREY);
				t = Text(font, alignment, Text::DARKGREY);
				b = Text(font, alignment, Text::DARKGREY);
			}

			OutlinedText() {}

			void draw(Point<int16_t> position) const
			{
				l.draw(position + Point<int16_t>(-1, 0));
				r.draw(position + Point<int16_t>(1, 0));
				t.draw(position + Point<int16_t>(0, -1));
				b.draw(position + Point<int16_t>(0, 1));
				inner.draw(position);
			}

			void settext(string text)
			{
				inner.settext(text);
				l.settext(text);
				r.settext(text);
				t.settext(text);
				b.settext(text);
			}
		};
		OutlinedText namelabel;

		static const size_t NUM_LABELS = 7;
		enum InfoLabel
		{
			JOB, WORLDRANK, JOBRANK,
			STR, DEX, INT, LUK
		};
		OutlinedText infolabels[NUM_LABELS];
	};

	// Factory for the character selection screen.
	class ElementCharSelect : public Element
	{
		UIElement::Type type() const override
		{
			return UIElement::CHARSELECT;
		}

		UIElement* instantiate() const override
		{
			return new UICharSelect();
		}
	};
}

