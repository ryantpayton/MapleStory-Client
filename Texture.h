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
#include "Graphic.h"
#include "BitmapWrapper.h"
#include "node.hpp"

using namespace nl;

namespace Graphics
{
	class Texture : public Graphic
	{
	public:
		Texture(node);
		Texture() { loaded = false; }
		~Texture() { if (loaded) delete source; }
		void draw(DrawArgument&);
		void update(short) {}
		void setshift(vector2d<int> s) { shift = s; }
		vector2d<int> getorigin() { return origin; }
		vector2d<int> getdimension() { return dimension; }
	private:
		BitmapWrapper* source;
		vector2d<int> origin;
		vector2d<int> dimension;
		vector2d<int> shift;
		bool loaded;
	};
}