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
#include "Gameplay\Physics\PhysicsObject.h"
#include "Graphics\Animation.h"
#include "Util\Point.h"

namespace Gameplay
{
	// Represents a projectile on a map.
	class Bullet
	{
	public:
		using Animation = Graphics::Animation;

		Bullet(Animation animation, Point<int16_t> origin, bool toleft);

		void draw(Point<int16_t> viewpos, float alpha) const;
		bool settarget(Point<int16_t> target);
		bool update(Point<int16_t> target);

	private:
		Animation animation;
		MovingObject moveobj;
		bool toleft;
		bool flip;
	};
}