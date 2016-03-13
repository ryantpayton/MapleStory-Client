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
#include "Foothold.h"

namespace Gameplay
{
	// Struct that contains all properties for movement calculations.
	struct MovingObject
	{
		double fx = 0.0;
		double fy = 0.0;
		double lastx = 0.0;
		double lasty = 0.0;
		double hspeed = 0.0;
		double vspeed = 0.0;

		void move()
		{
			lastx = fx;
			lasty = fy;
			fx += hspeed;
			fy += vspeed;
		}

		int16_t getx() const
		{
			return static_cast<int16_t>(fx);
		}

		int16_t gety() const
		{
			return static_cast<int16_t>(fy);
		}

		int16_t getx(float inter) const
		{
			auto interx = static_cast<int16_t>((1.0f - inter) * lastx + inter * fx);
			return interx;
		}

		int16_t gety(float inter) const
		{
			auto intery = static_cast<int16_t>((1.0f - inter) * lasty + inter * fy);
			return intery;
		}

		Point<int16_t> getposition(float inter) const
		{
			auto interx = getx(inter);
			auto intery = gety(inter);
			return Point<int16_t>(interx, intery);
		}
	};

	// Struct that contains all properties neccessary for physics calculations.
	struct PhysicsObject : public MovingObject
	{
		// Determines which physics engine to use.
		enum Type
		{
			NORMAL,
			ICE,
			SWIMMING,
			FLYING,
			FIXATED
		};

		enum Flag
		{
			NOGRAVITY = 0x0001,
			TURNATEDGES = 0x0002,
		};

		Type type = NORMAL;
		int32_t flags = 0;
		uint16_t fhid = 0;
		double fhslope = 0.0;
		int8_t fhlayer = 0;
		double groundbelow = 0.0;
		bool onground = true;
		bool enablejd = false;
		double mass = 1.0;

		double hforce = 0.0;
		double vforce = 0.0;
		double hacc = 0.0;
		double vacc = 0.0;

		bool flagset(Flag f)
		{
			return (flags & f) != 0;
		}

		bool flagnotset(Flag f)
		{
			return !flagset(f);
		}

		void setflag(Flag f)
		{
			flags |= f;
		}

		void clearflag(Flag f)
		{
			if (flagset(f))
				flags ^= f;
		}
	};
}

