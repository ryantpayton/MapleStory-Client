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
#include <vector>
#include <map>

namespace Graphics
{
	using std::vector;
	using std::pair;

	// Class which consists of multiple textures to make an Animation.
	class Animation
	{
	public:
		Animation(node source);
		Animation();

		bool update();
		bool update(uint16_t timestep);
		void reset();
		void draw(const DrawArgument& arguments, float inter) const;
		vector2d<int16_t> getorigin() const;
		vector2d<int16_t> getdimensions() const;

	private:
		float nextalphastep(uint8_t frame, uint16_t timestep) const;

		// Data
		vector<Texture> textures;
		vector<uint16_t> delays;
		vector<pair<uint8_t, uint8_t>> alphablends;
		bool animated;

		// Values for the current state.
		uint8_t frame;
		uint16_t elapsed;
		float alphastep;
		float alpha;

		// Values for interpolation.
		uint8_t lastframe;
		uint16_t lastelapsed;
		float lastalpha;
	};
}

