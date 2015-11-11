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
#include "Gameplay\Maplemap\Mapobject.h"
#include "Character\Look\CharLook.h"
#include "Graphics\Textlabel.h"
#include "Gameplay\Physics\PhysicsObject.h"

namespace Character
{
	using::Gameplay::Mapobject;
	using::Gameplay::PhysicsObject;
	using::Graphics::Textlabel;

	// Base for characters, e.g. the player and other clients on the same map.
	class Char : public Mapobject
	{
	public:
		// Player stances which determine animation and state. Values are used in movement packets (add 1 if facing left).
		enum Stance
		{
			WALK = 2,
			STAND = 4,
			FALL = 6,
			ALERT = 8,
			PRONE = 10,
			SWIM = 12,
			LADDER = 14,
			ROPE = 16,
			DIED = 18,
			SIT = 20,
			SKILL = 22
		};

		void setflip(bool);
		void setstance(Stance);
		void setposition(int32_t, int32_t);
		bool getflip() const;
		int8_t getlayer() const;
		int32_t getoid() const;
		vector2d<int32_t> getposition() const;
		Stance getstance() const;
		CharLook& getlook();
		PhysicsObject& getphobj();

	protected:
		void draw(vector2d<int32_t>, float) const;

		CharLook look;
		Textlabel namelabel;

		PhysicsObject phobj;
		int32_t cid;
		Stance stance;
		bool flip;
	};
}