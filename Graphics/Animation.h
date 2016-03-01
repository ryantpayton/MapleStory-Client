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
#include "Util\rectangle2d.h"
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

		Point<int16_t> getorigin() const;
		Point<int16_t> getdimensions() const;
		Point<int16_t> gethead() const;
		rectangle2d<int16_t> getbounds() const;

	private:
		struct Frame
		{
			Texture texture;
			uint16_t delay;
			pair<uint8_t, uint8_t> opacities;
			rectangle2d<int16_t> bounds;
			Point<int16_t> head;

			Frame(node src)
			{
				texture = src;
				bounds = src;
				head = src["head"];
				delay = src["delay"];
				if (delay == 0)
					delay = 50;

				uint8_t a0;
				uint8_t a1;
				node::type a0type = src["a0"].data_type();
				if (a0type == node::type::integer)
				{
					node::type a1type = src["a1"].data_type();
					if (a1type == node::type::integer)
					{
						a0 = src["a0"];
						a1 = src["a1"];
					}
					else
					{
						a0 = src["a0"];
						a1 = 255 - a0;
					}
				}
				else
				{
					a0 = 255;
					a1 = 255;
				}
				opacities = { a0, a1 };
			}

			Frame()
			{
				delay = 0;
				opacities = { 0, 0 };
			}

			float alphastep(float alpha, uint16_t timestep) const
			{
				return timestep * (opacities.second - alpha) / delay;
			}
		};

		// Data
		vector<Frame> frames;
		bool animated;
		bool zigzag;

		// Values for the current state.
		uint8_t frame;
		int8_t framestep;
		uint16_t elapsed;
		float alphastep;
		float alpha;

		// Values for interpolation.
		uint8_t lastframe;
		uint16_t lastelapsed;
		float lastalpha;
	};
}

