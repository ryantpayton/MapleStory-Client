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
#include "Graphics\Animation.h"
#include "Util\Point.h"

namespace Gameplay
{
	using Graphics::Animation;

	// Represents a projectile on a map.
	class Bullet
	{
	public:
		Bullet(Animation animation, Point<int16_t> origin, bool toleft);

		void draw(Point<int16_t> viewpos, float alpha) const;
		bool update(Point<int16_t> target);

	private:
		Point<int16_t> getposition(float alpha) const;

		Animation animation;
		bool flip;
		float fx;
		float fy;
		float lastx;
		float lasty;
		float hspeed;
		float vspeed;
	};
}