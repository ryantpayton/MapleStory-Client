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
#include "Gameplay\Physics\Physics.h"
#include "Gameplay\Camera.h"
#include "Graphics\Text.h"
#include "Graphics\Animation.h"
#include "Util\Point.h"
#include <cstdint>
#include <string>
#include <map>

namespace Character
{
	using std::string;
	using std::map;
	using Gameplay::Physics;
	using Gameplay::PhysicsObject;
	using Gameplay::Camera;
	using Graphics::Text;
	using Graphics::Animation;

	class PetLook
	{
	public:
		enum Stance
		{
			STAND = 0,
			MOVE = 2,
			HANG = 4,
			FLY = 6
		};

		PetLook(int32_t iid, string name, int32_t uniqueid, 
			Point<int16_t> pos, uint8_t stance, int32_t fhid);
		PetLook();

		void draw(Point<int16_t> viewpos, float inter) const;
		void update(const Physics& physics, Point<int16_t> charpos);

		void setposition(int16_t xpos, int16_t ypos);
		void setstance(Stance);

		int32_t getiid() const;
		Stance getstance() const;

	private:
		int32_t itemid;
		string name;
		int32_t uniqueid;
		Stance stance;
		bool flip;

		map<string, Animation> animations;
		PhysicsObject phobj;
		Text namelabel;
	};
}