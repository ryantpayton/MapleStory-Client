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
#include "GraphicsEngine.h"

#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#include "GraphicsGL.h"
#else
#include "GraphicsD2D.h"
#endif

namespace Graphics
{
	namespace GraphicsEngine
	{
		void addbitmap(const bitmap& bmp)
		{
#ifdef JOURNEY_USE_OPENGL
			GraphicsGL::addbitmap(bmp);
#else
			GraphicsD2D::addbitmap(bmp);
#endif
		}

		bool available(size_t id)
		{
#ifdef JOURNEY_USE_OPENGL
			return GraphicsGL::available(id);
#else
			return GraphicsD2D::available(id);
#endif
		}

		void draw(size_t id, int16_t x, int16_t y, int16_t w, int16_t h,
			float alpha, float xscale, float yscale, int16_t centerx, int16_t centery) {

#ifdef JOURNEY_USE_OPENGL
			GraphicsGL::draw(id, x, y, w, h, alpha, xscale, yscale, centerx, centery);
#else
			GraphicsD2D::draw(id, x, y, w, h, alpha, xscale, yscale, centerx, centery);
#endif
		}

		void clear()
		{
#ifdef JOURNEY_USE_OPENGL
			GraphicsGL::clear();
#else
			GraphicsD2D::clear();
#endif
		}
	}
}