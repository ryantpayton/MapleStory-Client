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
#include "UIElement.h"

namespace IO
{
	class Element
	{
	public:
		virtual ~Element() {}

		// Add the element to the current UI state.
		void add() const;

		// Return wether the element can only be created once.
		// Such elements will have their visibility toggled when adding them again.
		// Default: false
		virtual bool isunique() const;

		// Return wether the element is focused.
		// These elements always stay on top of the screen.
		// Default: false
		virtual bool isfocused() const;

		// Return the type of this element.
		virtual UIElement::Type type() const = 0;

		// Create the Element instance.
		virtual UIElement* instantiate() const = 0;
	};
}