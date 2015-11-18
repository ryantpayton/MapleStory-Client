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
#include "Mapobject.h"
#include "Gameplay\Physics\PhysicsObject.h"
#include "Graphics\Animation.h"
#include "Graphics\Textlabel.h"
#include "Util\rectangle2d.h"
#include "Util\Randomizer.h"

namespace Gameplay
{
	using std::string;
	using std::map;
	using Util::rectangle2d;
	using Util::Randomizer;
	using Graphics::Animation;
	using Graphics::Textlabel;

	class Mob : public MapObject
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

		// Construct a mob by combining data from game files with
		// data sent by the server.
		Mob(int32_t oid, int32_t mobid, bool control, int8_t stance, 
			uint16_t fhid, int8_t effect, bool newspawn, int8_t team, 
			int16_t xpos, int16_t ypos);

		// Update movement and animations.
		int8_t update(const Physics& physics) override;
		// Draw the object.
		void draw(vector2d<int16_t> viewpos, float inter) const override;
		// Change position.
		void setposition(int16_t xpos, int16_t ypos) override;
		// Return the layer of the current platform.
		int8_t getlayer() const override;
		// Return object id.
		int32_t getoid() const override;
		// Return position.
		vector2d<int16_t> getposition() const override;

		// Kill the mob with the appropriate type:
		// 0: make inactive 1: death animation 2: fade out
		void kill(int8_t killtype);
		// Display the hp percentage above the mob.
		// Use the playerlevel to determine color of nametag.
		void sendhp(int8_t percentage, uint16_t playerlevel);
		// Make an inactive (out of viewrange) mob active again.
		void makeactive();

	private:
		void parsestance(Stance toparse, node source);
		//void parsesound(Stance toparse, node source);
		void nextmove();
		void setstance(Stance newstance);

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

