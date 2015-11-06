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
#include "DrawArgument.h"

namespace Graphics
{
	using::std::uint16_t;
	// Interface for classes that represent textures or animations.
	class Graphic
	{
	public:
		virtual ~Graphic() {}
		virtual void update(uint16_t) = 0;
		virtual void draw(const DrawArgument&) const = 0;
		virtual vector2d<int32_t> getorigin() const = 0;
		virtual vector2d<int32_t> getdimensions() const = 0;
	};
}