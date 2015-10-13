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
#include "bitmap.hpp"
#include "rectangle2d.h"

using namespace nl;
using namespace Util;

namespace Program
{
	enum Imagecontext
	{
		ICT_SYSTEM,
		ICT_LOGIN,
		ICT_MAP
	};

	class Imagecache
	{
	public:
		virtual ~Imagecache() {}
		virtual Imagecontext createimage(bitmap) = 0;
		virtual void draw(Imagecontext, size_t, rectangle2d<int32_t>, float_t, float_t, vector2d<int32_t>, float_t) = 0;
		virtual void setmode(Imagecontext) = 0;
		virtual void unlock() = 0;
		virtual void clearcache(Imagecontext) = 0;
	};
}