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
#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#include "Util\rectangle2d.h"
#include "Text.h"
#include "nlnx\bitmap.hpp"

namespace Graphics
{
	using nl::bitmap;
	using Util::vector2d;
	using Util::rectangle2d;

	// Graphic functions for usage with OpenGL.
	namespace GraphicsGL
	{
		// Initialise all resources.
		bool init();
		// Clear all bitmaps.
		void clear();
		// Clear all resources.
		void free();

		// Add a bitmap to the available resources.
		void addbitmap(const bitmap& toadd);
		// Return wether the bitmap with the given id is in the resource pool.
		bool available(size_t id);
		// Draw the bitmap with the given id and paramters.
		void draw(size_t id, int16_t x, int16_t y, int16_t w, 
			int16_t h, float, float, float, int16_t centerx, int16_t centery);

		// Create a layout for the text with the parameters specified.
		Text::Layout createlayout(const char* text, Text::Font font, float maxwidth);
		// Draw a text with the given parameters.
		void drawtext(const char* text, Text::Font font, Text::Alignment alignment, Text::Color color,
			Text::Background back, float opacity, vector2d<float> origin, vector2d<float> layout);
	};
}
#endif

