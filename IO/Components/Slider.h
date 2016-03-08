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
#include "Button.h"
#include "IO\Cursor.h"
#include "Graphics\Texture.h"
#include <functional>
#include <memory>

namespace IO
{
	using std::function;
	using std::unique_ptr;
	using std::pair;
	using Graphics::Texture;

	class Slider
	{
	public:
		Slider(int32_t type, Range<int16_t> vertical, int16_t x, int16_t rows, function<void(bool upwards)> onmoved);

		bool isenabled() const;
		void setenabled(bool enabled);
		void setrows(int16_t rows);

		void draw(Point<int16_t> position) const;
		Cursor::State sendcursor(Point<int16_t> cursor, bool pressed);

	private:
		Point<int16_t> getmidpos() const;

		function<void(bool upwards)> onmoved;

		Range<int16_t> vertical;
		int16_t buttonheight;
		int16_t rowheight;
		int16_t x;
		int16_t row;
		int16_t rowmax;
		bool scrolling;
		bool enabled;

		Point<int16_t> start;
		Point<int16_t> end;

		Texture dbase;
		Texture dnext;
		Texture dprev;

		Texture base;

		unique_ptr<Button> next;
		unique_ptr<Button> prev;
		unique_ptr<Button> thumb;
	};
}