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
#include "..\..\Constants.h"
#include "..\..\Util\Interpolated.h"
#include "..\..\Util\Point.h"

namespace jrc
{
	// Struct that contains all properties for movement calculations.
	struct MovingObject
	{
		Linear<double> x;
		Linear<double> y;
		double hspeed = 0.0;
		double vspeed = 0.0;

		void normalize()
		{
			x.normalize();
			y.normalize();
		}

		void move()
		{
			x += hspeed;
			y += vspeed;
		}

		void setx(double d)
		{
			x.set(d);
		}

		void sety(double d)
		{
			y.set(d);
		}

		void limitx(double d)
		{
			x = d;
			hspeed = 0.0;
		}

		void limity(double d)
		{
			y = d;
			vspeed = 0.0;
		}

		void movexuntil(double d, uint16_t delay)
		{
			if (delay)
			{
				double hdelta = d - x.get();
				hspeed = Constants::TIMESTEP * hdelta / delay;
			}
		}

		void moveyuntil(double d, uint16_t delay)
		{
			if (delay)
			{
				double vdelta = d - y.get();
				vspeed = Constants::TIMESTEP * vdelta / delay;
			}
		}

		bool hmobile() const
		{
			return hspeed != 0.0;
		}

		bool vmobile() const
		{
			return vspeed != 0.0;
		}

		bool mobile() const
		{
			return hmobile() || vmobile();
		}

		double crntx() const
		{
			return x.get();
		}

		double crnty() const
		{
			return y.get();
		}

		double nextx() const
		{
			return x + hspeed;
		}

		double nexty() const
		{
			return y + vspeed;
		}

		int16_t getx() const
		{
			double rounded = std::round(x.get());
			return static_cast<int16_t>(rounded);
		}

		int16_t gety() const
		{
			double rounded = std::round(y.get());
			return static_cast<int16_t>(rounded);
		}

		int16_t lastx() const
		{
			double rounded = std::round(x.last());
			return static_cast<int16_t>(rounded);
		}

		int16_t lasty() const
		{
			double rounded = std::round(y.last());
			return static_cast<int16_t>(rounded);
		}

		Point<int16_t> getposition() const
		{
			return{ getx(), gety() };
		}

		int16_t getabsx(double viewx, float alpha) const
		{
			double interx = x.normalized() ? std::round(x.get()) : x.get(alpha);
			return static_cast<int16_t>(
				std::round(interx + viewx)
				);
		}

		int16_t getabsy(double viewy, float alpha) const
		{
			double intery = y.normalized() ? std::round(y.get()) : y.get(alpha);
			return static_cast<int16_t>(
				std::round(intery + viewy)
				);
		}

		Point<int16_t> getabsolute(double viewx, double viewy, float alpha) const
		{
			return{ getabsx(viewx, alpha), getabsy(viewy, alpha) };
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
			CHECKBELOW = 0x0004
		};

		Type type = NORMAL;
		int32_t flags = 0;
		uint16_t fhid = 0;
		double fhslope = 0.0;
		int8_t fhlayer = 0;
		double groundbelow = 0.0;
		bool onground = true;
		bool enablejd = false;

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

		void clearflags()
		{
			flags = 0;
		}
	};
}

