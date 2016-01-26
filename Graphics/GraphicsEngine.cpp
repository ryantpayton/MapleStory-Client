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
	void GraphicsEngine::addbitmap(const bitmap& bmp)
	{
#ifdef JOURNEY_USE_OPENGL
		GraphicsGL::get().addbitmap(bmp);
#else
		GraphicsD2D::get().addbitmap(bmp);
#endif
	}

	bool GraphicsEngine::available(size_t id)
	{
#ifdef JOURNEY_USE_OPENGL
		return GraphicsGL::get().available(id);
#else
		return GraphicsD2D::get().available(id);
#endif
	}

	void GraphicsEngine::draw(size_t id, int16_t x, int16_t y, int16_t w, int16_t h,
		float a, float xs, float ys, int16_t cx, int16_t cy) {
#ifdef JOURNEY_USE_OPENGL
		GraphicsGL::get().draw(id, x, y, w, h, a, xs, ys, cx, cy);
#else
		GraphicsD2D::get().draw(id, x, y, w, h, a, xs, ys, cx, cy);
#endif
	}

	void GraphicsEngine::clear()
	{
#ifdef JOURNEY_USE_OPENGL
		GraphicsGL::get().clear();
#else
		GraphicsD2D::get().clear();
#endif
	}

	Text::Layout GraphicsEngine::createlayout(const string& text, Text::Font font, float maxwidth)
	{
#ifdef JOURNEY_USE_OPENGL
		return GraphicsGL::get().createlayout(text.c_str(), font, maxwidth);
#else
		wstring wtext = wstring(text.begin(), text.end());
		return GraphicsD2D::get().createlayout(wtext, font, maxwidth);
#endif
	}

	void GraphicsEngine::drawtext(const string& text, Text::Font font, Text::Alignment alignment, Text::Color color,
		Text::Background back, float alpha, vector2d<float> origin, vector2d<float> dimensions) {
#ifdef JOURNEY_USE_OPENGL
		GraphicsGL::get().drawtext(text.c_str(), font, alignment, color, back, alpha, origin, dimensions);
#else
		wstring wtext = wstring(text.begin(), text.end());
		GraphicsD2D::get().drawtext(wtext, font, alignment, color, back, alpha, origin, dimensions);
#endif
	}
}