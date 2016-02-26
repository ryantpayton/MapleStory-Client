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
#include "Util\Singleton.h"
#include "Text.h"
#include "nlnx\bitmap.hpp"

namespace Graphics
{
	using nl::bitmap;

	class GraphicsEngine : public Singleton<GraphicsEngine>
	{
	public:
		void clear();

		bool available(size_t id);
		void addbitmap(const bitmap& bmp);
		void draw(size_t id, int16_t x, int16_t y, int16_t w, int16_t h, 
			float alpha, float xscale, float yscale, int16_t centerx, int16_t centery);

		Text::Layout createlayout(const string& text, Text::Font font, Text::Alignment alignment, int16_t maxwidth);
		void drawtext(const string& text, const Text::Layout& layout, Text::Font font, 
			Text::Color color, Text::Background back, vector2d<int16_t> origin, float alpha);
	};
}