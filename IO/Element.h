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
#include "IO\UIElement.h"

namespace IO
{
	class Element
	{
	public:
		enum UIType
		{
			NONE,
			LOGIN,
			LOGINWAIT,
			LOGINNOTICE,
			WORLDSELECT,
			CHARSELECT,
			CHARCREATION,
			SOFTKEYBOARD,
			STATUSBAR,
			STATSINFO,
			ITEMINVENTORY,
			EQUIPINVENTORY,
		};

		virtual ~Element() {}
		virtual bool isunique() const { return false; }
		virtual bool isfocused() const { return false; }
		virtual UIType type() const = 0;
		virtual UIElement* instantiate() const = 0;
	};
}