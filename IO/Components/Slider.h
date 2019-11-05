//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "TwoSpriteButton.h"

#include "../Cursor.h"

#include "Graphics/Texture.h"

#include <functional>
#include <memory>

namespace ms
{
	class Slider
	{
	public:
		Slider(std::int32_t type, Range<int16_t> vertical, std::int16_t x, std::int16_t unitrows, std::int16_t rowmax, std::function<void(bool upwards)> onmoved);
		Slider();

		bool isenabled() const;
		void setenabled(bool enabled);
		void setrows(std::int16_t newrow, std::int16_t unitrows, std::int16_t rowmax);
		void setrows(std::int16_t unitrows, std::int16_t rowmax);
		void setvertical(Range<int16_t> vertical);

		void draw(Point<int16_t> position) const;
		bool remove_cursor(bool clicked);
		Cursor::State send_cursor(Point<int16_t> cursor, bool pressed);
		void send_scroll(double yoffset);

		enum Type
		{
			BLUE,
			BROWN = 2,
			ICE,
			BLUE2,
			BLUE3,
			GOLD,
			GOLD2,
			BROWN2,
			DEFAULT,
			GOLD3,
			DEFAULT2,
			BROWN3,
			BROWN4,
			RED,
			THIN,
			THIN2,
			THIN3,
			THIN4,
			THIN5,
			ICE2,
			DEFAULT3 = 100,
			THIN6,
			ARROW,
			THIN7,
			ICE3,
			THIN8,
			CHATBAR
		};

	private:
		Point<int16_t> getthumbpos() const;

		std::function<void(bool upwards)> onmoved;

		Range<int16_t> vertical;
		Point<int16_t> start;
		Point<int16_t> end;
		std::int16_t type;
		std::int16_t buttonheight;
		std::int16_t rowheight;
		std::int16_t x;
		std::int16_t row;
		std::int16_t rowmax;
		bool scrolling;
		bool enabled;

		Texture dbase;
		Texture dnext;
		Texture dprev;
		Texture base;
		TwoSpriteButton next;
		TwoSpriteButton prev;
		TwoSpriteButton thumb;
	};
}