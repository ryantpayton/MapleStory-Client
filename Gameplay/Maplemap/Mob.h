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
#include "Gameplay\Attack.h"
#include "Gameplay\Physics\PhysicsObject.h"
#include "Gameplay\MovementInfo.h"
#include "Graphics\Animation.h"
#include "Graphics\Textlabel.h"
#include "IO\Components\DamageNumber.h"
#include "Util\rectangle2d.h"
#include "Util\Randomizer.h"
#include "Net\Code\SeededState.h"

namespace Gameplay
{
	using std::string;
	using std::pair;
	using std::vector;
	using std::map;
	using Util::rectangle2d;
	using Util::Randomizer;
	using Net::SeededState;
	using Graphics::Animation;
	using Graphics::Textlabel;
	using IO::DamageNumber;

	class Mob : public MapObject
	{
	public:
		enum Stance
		{
			MOVE = 2,
			STAND = 4,
			JUMP = 6,
			HIT = 8,
			DIE = 10
		};

		enum Behaviour
		{
			MOVELEFT,
			MOVERIGHT,
			STOP
		};

		// Construct a mob by combining data from game files with
		// data sent by the server.
		Mob(int32_t oid, int32_t mobid, int32_t seed, int8_t stance, uint16_t fhid, 
			bool newspawn, int8_t team, int16_t xpos, int16_t ypos);

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

		bool isactive() const;
		bool isinrange(const rectangle2d<int16_t>& range) const;

		vector<int32_t> damage(const Attack& attack);

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
		void setstance(Stance newstance);

		pair<int32_t, bool> randomdamage(int32_t mindamage, 
			int32_t maxdamage, float hitchance, float critical) const;

		map<Stance, Animation> animations;
		map<Stance, rectangle2d<int16_t>> bounds;
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
		vector<DamageNumber> damagenumbers;

		SeededState behaviour;
		uint16_t counter;

		int32_t oid;
		int32_t id;
		int8_t effect;
		int8_t team;

		bool active;
		Stance stance;
		PhysicsObject phobj;
		bool flip;
		float walkforce;
		int8_t hppercent;
		bool fading;
		bool fadein;
		float alpha;
	};
}

