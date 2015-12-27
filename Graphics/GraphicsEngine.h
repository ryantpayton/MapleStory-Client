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
#include "nlnx\bitmap.hpp"

namespace Graphics
{
	namespace GraphicsEngine
	{
		using nl::bitmap;

		void addbitmap(const bitmap& bmp);
		void draw(size_t id, int16_t x, int16_t y, int16_t w, int16_t h, 
			float alpha, float xscale, float yscale, int16_t centerx, int16_t centery);
		bool available(size_t id);
		void clear();
	}
}