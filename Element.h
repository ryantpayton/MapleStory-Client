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
#include "UIElement.h"

namespace IO
{
	enum UIType
	{
		UI_LOGIN,
		UI_LOGINWAIT,
		UI_LOGINNOTICE,
		UI_WORLDSELECT,
		UI_CHARSELECT,
		UI_CHARCREATION,
		UI_SOFTKEYBOARD
	};

	class Element
	{
	public:
		virtual ~Element() {}
		virtual bool isunique() const { return false; }
		virtual bool isfocused() const { return false; }
		virtual UIType type() const = 0;
		virtual UIElement* instantiate() const = 0;
	};
}