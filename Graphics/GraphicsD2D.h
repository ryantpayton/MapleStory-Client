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
#include "Graphics\Textlabel.h"
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

		void addbitmap(const bitmap& toadd);
		bool available(size_t id);
		void draw(size_t, int16_t, int16_t, int16_t, int16_t, float, float, float, int16_t, int16_t);

		vector2d<uint16_t> createlayout(const wstring&, Textlabel::Font, uint16_t, map<size_t, float>&);
		void drawtext(const wstring&, Textlabel::Font, Textlabel::Textcolor, 
			Textlabel::Background, float, vector2d<float>, vector2d<uint16_t>);
	};
}
#endif