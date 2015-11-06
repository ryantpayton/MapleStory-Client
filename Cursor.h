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
#include "InputConstants.h"
#include "Animation.h"
#include "Ptrmap.h"

namespace IO
{
	using::std::uint16_t;
	using::std::int32_t;
	using::Util::vector2d;
	using::Util::Ptrmap;
	using::Graphics::Animation;

	class Cursor
	{
	public:
		Cursor();
		~Cursor();
		void init();
		void draw() const;
		void update(uint16_t);
		void setstate(Mousestate);
		void setposition(vector2d<int32_t>);
		Mousestate getstate() const;
	private:
		Ptrmap<Mousestate, Animation> animations;
		Mousestate state;
		vector2d<int32_t> position;
	};
}

