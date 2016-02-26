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
#include "Util\Singleton.h"
#include "Util\rectangle2d.h"
#include "Graphics\Text.h"
#include "nlnx\bitmap.hpp"
#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodecsdk.h>
#include <dwrite.h>
#include <unordered_map>

namespace Graphics
{
	using std::unordered_map;
	using std::wstring;
	using nl::bitmap;

	class GraphicsD2D : public Singleton<GraphicsD2D>
	{
	public:
		GraphicsD2D();
		~GraphicsD2D();

		void init(IWICImagingFactory**, ID2D1BitmapRenderTarget**, IDWriteFactory**);
		void clear();
		void free();

		void addbitmap(const bitmap& toadd);
		bool available(size_t id);
		void draw(size_t id, int16_t x, int16_t y, int16_t w, int16_t h, float alpha, 
			float xscale, float yscale, int16_t originx, int16_t originy);

		Text::Layout createlayout(const wstring& text, Text::Font font, float maxwidth);
		void drawtext(const wstring& text, const Text::Layout& layout, Text::Font font, 
			Text::Color color, Text::Background back, vector2d<int16_t> origin, float alpha);

	private:
		void addfont(Text::Font id, const WCHAR* name, float size, DWRITE_FONT_WEIGHT weight);

		IWICImagingFactory** p_factory;
		ID2D1BitmapRenderTarget** p_target;
		IDWriteFactory** p_dwfactory;

		unordered_map<size_t, ID2D1Bitmap*> bitmaps;

		ID2D1SolidColorBrush* brushes[Text::NUM_COLORS];
		IDWriteTextFormat* fonts[Text::NUM_FONTS];
	};
}
#endif