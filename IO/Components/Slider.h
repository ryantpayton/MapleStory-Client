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
#include "Graphics\Texture.h"
#include "nlnx\node.hpp"

namespace IO
{
	using nl::node;
	using Graphics::Texture;

	class Slider
	{
	public:
		Slider(node src, vector2d<int16_t> vertical, int16_t x);
		Slider();
		~Slider();

		bool isenabled() const;
		void setenabled(bool enabled);
		void draw(vector2d<int16_t> position) const;

	private:
		vector2d<int16_t> vertical;
		int16_t x;
		int16_t slidery;
		bool enabled;

		Texture dbase;
		Texture dnext;
		Texture dprev;

		Texture base;
		Texture next0;
		Texture next1;
		Texture prev0;
		Texture prev1;
		Texture thump0;
		Texture thump1;
	};
}