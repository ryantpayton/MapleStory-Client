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
		void removechar(uint8_t);

	private:
		void addchar(uint8_t);
		void selectchar();
		vector2d<int16_t> getcharpos(size_t) const;

		bool haschars;
		vector<CharLook> charlooks;
		uint8_t charcount;
		uint8_t charslots;
		uint8_t selected;
		uint8_t page;
		Text namelabel;
		Text joblabel;
		vector<Nametag> nametags;
		Charset levelset;
		Charset statsset;
	};

	// Factory for the character selection screen.
	class ElementCharSelect : public Element
	{
		UIType type() const override
		{
			return CHARSELECT;
		}

		UIElement* instantiate() const override
		{
			return new UICharSelect();
		}
	};
}

