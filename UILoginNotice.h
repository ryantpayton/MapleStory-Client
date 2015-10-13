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
#include "Element.h"

namespace IO
{
	enum UILoginNoticeButtons
	{
		BT_OK
	};

	class UILoginNotice : public UIElement
	{
	public:
		UILoginNotice(char);
		void buttonpressed(short);
	private:
		bool saveid;
	};

	class ElementLoginNotice : public Element
	{
	public:
		ElementLoginNotice(char t)
		{
			text = t;
		}

		UIType type()
		{
			return UI_LOGINNOTICE;
		}

		UIElement* instantiate()
		{
			return new UILoginNotice(text);
		}
	private:
		char text;
	};
}