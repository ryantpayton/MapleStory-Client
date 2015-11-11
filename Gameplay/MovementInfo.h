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
#include <vector>

namespace Gameplay
{
	using::std::uint8_t;
	using::std::int16_t;
	using::std::uint16_t;
	using::std::vector;
	using::Gameplay::PhysicsObject;

	// Contains information on a single movement.
	struct MovementFragment
	{
		// Movement types for servers and other clients to handle.
		enum MovementType
		{
			MVT_NONE,
			MVT_ABSOLUTE,
			MVT_RELATIVE,
			MVT_CHAIR,
			MVT_JUMPDOWN
		};

		MovementType type;
		uint8_t command;
		int16_t xpos;
		int16_t ypos;
		int16_t lastx;
		int16_t lasty;
		uint16_t fh;
		uint8_t newstate;
		int16_t duration;
	};

	// Class that stores multiple movement fragments.
	// This is neccessary for updating the movement of controlled objects with the server.
	class MovementInfo
	{
	public:
		MovementInfo() {}
		~MovementInfo() {}

		// Add a movement to the vector of store movements.
		void addmovement(const PhysicsObject& phobj, uint8_t command, uint8_t state, int16_t duration)
		{
			MovementFragment toadd;
			toadd.type = MovementFragment::MVT_ABSOLUTE;
			toadd.command = command;
			toadd.xpos = static_cast<int16_t>(phobj.fx);
			toadd.ypos = static_cast<int16_t>(phobj.fy);
			toadd.lastx = static_cast<int16_t>(phobj.lastx);
			toadd.lasty = static_cast<int16_t>(phobj.lasty);
			toadd.fh = phobj.fhid;
			toadd.newstate = state;
			toadd.duration = duration;
			fragments.push_back(toadd);
		}

		// Add a movement to the vector of store movements.
		void addmovement(MovementFragment toadd)
		{
			fragments.push_back(toadd);
		}

		// Return a begin iterator for iteration.
		vector<MovementFragment>::const_iterator getbegin() const
		{
			return fragments.begin();
		}

		// Return an end iterator for iteration.
		vector<MovementFragment>::const_iterator getend() const
		{
			return fragments.end();
		}

		// Return the last movement stored.
		const MovementFragment& gettop() const
		{
			return fragments.back();
		}

		// Return the amount of movements stored. 
		size_t getsize() const
		{
			return fragments.size();
		}

		// Clear the fragments vector. That way a MovementInfo can be reused as a member variable.
		void clear()
		{
			fragments.clear();
		}
	private:
		vector<MovementFragment> fragments;
	};
}