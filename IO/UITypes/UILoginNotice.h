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

namespace IO
{
	class UILoginNotice : public UIElement
	{
	public:
		enum Buttons
		{
			BT_OK
		};

		UILoginNotice(int8_t);
		void buttonpressed(uint16_t) override;

	private:
		bool saveid;
	};

	class ElementLoginNotice : public Element
	{
	public:
		ElementLoginNotice(int8_t t)
		{
			text = t;
		}

		bool isfocused() const override
		{
			return true;
		}

		UIElement::Type type() const override
		{
			return UIElement::LOGINNOTICE;
		}

		UIElement* instantiate() const override
		{
			return new UILoginNotice(text);
		}
	private:
		int8_t text;
	};
}