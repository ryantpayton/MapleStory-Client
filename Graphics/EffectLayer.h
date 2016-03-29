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
#include "Animation.h"
#include <map>
#include <list>

namespace Graphics
{
	using std::map;
	using std::list;

	// A list of animations. Animations will be removed after all frames were displayed.
	class EffectLayer
	{
	public:
		void drawbelow(Point<int16_t> position, float alpha) const;
		void drawabove(Point<int16_t> position, float alpha) const;
		void update();
		void add(Animation effect, bool flip, int8_t z, float speed);
		void add(Animation effect, bool flip, int8_t z);
		void add(Animation effect, bool flip);
		void add(Animation effect);

	private:
		class Effect
		{
		public:
			friend class EffectLayer;

		protected:
			Effect(Animation a, uint16_t ts, bool f) : animation(a), timestep(ts), flip(f) {}

			void draw(Point<int16_t> position, float alpha) const
			{
				animation.draw(DrawArgument(position, flip), alpha);
			}

			bool update()
			{
				return animation.update(timestep);
			}

		private:
			Animation animation;
			uint16_t timestep;
			bool flip;
		};
		map<int8_t, list<Effect>> effectsbelow;
		map<int8_t, list<Effect>> effectsabove;
	};
}