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
#include "Gameplay\Physics\PhysicsObject.h"
#include "Graphics\Animation.h"
#include "Graphics\Textlabel.h"
#include "Util\rectangle2d.h"
#include "Util\Randomizer.h"

namespace Gameplay
{
	using::std::string;
	using::Util::rectangle2d;
	using::Util::Randomizer;
	using::Graphics::Animation;
	using::Graphics::Textlabel;

	class Mob : public Mapobject
	{
	public:
		enum Stance
		{
			STAND = 2,
			MOVE = 4,
			JUMP = 6,
			HIT = 8,
			DIE = 10
		};

		Mob(int32_t, int32_t, bool, int8_t, uint16_t, int8_t, bool, int8_t, int32_t, int32_t);
		~Mob();

		int8_t update(const Physics&);
		void draw(vector2d<int32_t>, float) const;
		void kill(int8_t);
		void sendhp(int8_t, uint16_t);
		void makeactive();

		void setposition(int32_t, int32_t);
		int8_t getlayer() const;
		int32_t getoid() const;
		vector2d<int32_t> getposition() const;

	private:
		void parsestance(Stance, node);
		//void parsesound(mobstate, node);
		void nextmove();
		void setstance(Stance);

		map<Stance, Animation> animations;
		map<Stance, rectangle2d<int32_t>> bounds;
		string name;
		uint16_t level;
		uint16_t speed;
		uint16_t watk;
		uint16_t matk;
		uint16_t wdef;
		uint16_t mdef;
		uint16_t accuracy;
		uint16_t avoid;
		uint16_t knockback;
		bool undead;
		bool touchdamage;

		Textlabel namelabel;
		Randomizer randomizer;

		int32_t oid;
		int32_t id;
		bool control;
		int8_t effect;
		int8_t team;

		bool active;
		Stance stance;
		PhysicsObject phobj;
		uint16_t moved;
		bool flip;
		float walkforce;
		int8_t hppercent;
		bool fading;
	};
}

