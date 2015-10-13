/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "Texture.h"
#include "ptrvector.h"

namespace Graphics
{
	const short DEF_DELAY = 50;

	class Animation : public Graphic
	{
	public:
		Animation(node);
		~Animation() {}
		void draw(DrawArgument*);
		void update(short);
		vector2d<int> getorigin();
		vector2d<int> getdimension();
	private:
		ptrvector<Texture*> frames;
		vector<short> delays;
		vector<pair<uint8_t, uint8_t>> alphablends;
		uint8_t frame;
		uint8_t last_f;
		bool blending;
		short elapsed;
		float alphastep;
		float alpha;
	};
}

