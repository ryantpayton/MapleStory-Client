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
#include "Util\vector2d.h"
#include <cstdint>

namespace Gameplay
{
	using std::int8_t;
	using std::int16_t;
	using std::uint16_t;
	using Util::vector2d;

	// Struct that contains all properties neccessary for physics calculations.
	struct PhysicsObject
	{
		// Determines which physics engine to use.
		enum PhType
		{
			NORMAL,
			ICE,
			SWIMMING,
			FLYING,
			CLIMBING,
		};

		PhType type = NORMAL;
		uint16_t fhid = 0;
		double fhslope = 0.0f;
		int8_t fhlayer = 0;
		bool onground = true;
		double mass = 1.0f;
		double fx = 0.0f;
		double fy = 0.0f;
		double hforce = 0.0f;
		double vforce = 0.0f;
		double hacc = 0.0f;
		double vacc = 0.0f;
		double hspeed = 0.0f;
		double vspeed = 0.0f;

		double lastx = 0.0f;
		double lasty = 0.0f;

		vector2d<int16_t> getposition(float inter) const
		{
			int16_t interx = static_cast<int16_t>((1.0f - inter) * lastx + inter * fx);
			int16_t intery = static_cast<int16_t>((1.0f - inter) * lasty + inter * fy);
			return vector2d<int16_t>(interx, intery);
		}
	};
}

