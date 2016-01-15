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
#ifndef JOURNEY_USE_OPENGL
#include "Graphics\Text.h"
#include "Util\rectangle2d.h"
#include "nlnx\bitmap.hpp"
#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodecsdk.h>
#include <dwrite.h>

namespace Graphics
{
	using::nl::bitmap;

	namespace GraphicsD2D
	{
		void init(IWICImagingFactory**, ID2D1BitmapRenderTarget**, IDWriteFactory**);
		void clear();
		void free();

		void addbitmap(const bitmap& toadd);
		bool available(size_t id);
		void draw(size_t id, int16_t x, int16_t y, int16_t w, int16_t h, float alpha, 
			float xscale, float yscale, int16_t originx, int16_t originy);

		Text::Layout createlayout(const wstring& text, Text::Font font, float maxwidth);
		void drawtext(const wstring& text, Text::Font font, Text::Alignment alignment, Text::Color color, 
			Text::Background back, float alpha, vector2d<float> origin, vector2d<float> dimensions);
	};
}
#endif