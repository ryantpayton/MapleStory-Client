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
#include "..\..\Gameplay\Physics\Physics.h"
#include "..\..\Graphics\Text.h"
#include "..\..\Graphics\Animation.h"
#include "..\..\Util\EnumMap.h"
#include "..\..\Util\Point.h"

#include <cstdint>
#include <string>
#include <map>

namespace jrc
{
	class PetLook
	{
	public:
		enum Stance : uint8_t
		{
			MOVE,
			STAND,
			JUMP,
			ALERT,
			PRONE,
			FLY,
			HANG,
			WARP,
			LENGTH
		};

		static Stance stancebyvalue(uint8_t value)
		{
			uint8_t valueh = value / 2;
			return valueh >= LENGTH ? STAND : static_cast<Stance>(valueh);
		}

		PetLook(int32_t iid, std::string name, int32_t uniqueid,
			Point<int16_t> pos, uint8_t stance, int32_t fhid);
		PetLook();

		void draw(double viewx, double viewy, float alpha) const;
		void update(const Physics& physics, Point<int16_t> charpos);

		void setposition(int16_t xpos, int16_t ypos);
		void setstance(Stance stance);
		void setstance(uint8_t stancebyte);

		int32_t getiid() const;
		Stance getstance() const;

	private:
		int32_t itemid;
		std::string name;
		int32_t uniqueid;
		Stance stance;
		bool flip;

		EnumMap<Stance, Animation> animations;
		PhysicsObject phobj;
		Text namelabel;
	};
}