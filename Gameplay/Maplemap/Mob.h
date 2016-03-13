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
#include "Gameplay\Bullet.h"
#include "Gameplay\Physics\PhysicsObject.h"
#include "Graphics\Text.h"
#include "Graphics\EffectLayer.h"
#include "Graphics\Geometry\MobHpBar.h"
#include "Audio\Sound.h"
#include "IO\Components\DamageNumber.h"
#include "Util\rectangle2d.h"
#include "Util\Randomizer.h"
#include "Util\Enum.h"
#include <list>

namespace Gameplay
{
	using std::string;
	using std::pair;
	using std::vector;
	using std::list;
	using std::map;
	using Graphics::Text;
	using Graphics::EffectLayer;
	using Graphics::MobHpBar;
	using IO::DamageNumber;

	class Mob : public MapObject
	{
	public:
		static const size_t NUM_STANCES = 6;
		enum Stance : uint8_t
		{
			MOVE = 2,
			STAND = 4,
			JUMP = 6,
			HIT = 8,
			DIE = 10
		};

		static string nameof(Stance stance)
		{
			static const string stancenames[NUM_STANCES] =
			{
				"move", "stand", "jump", "hit1", "die1", "fly"
			};
			size_t index = (stance - 1) / 2;
			return stancenames[index];
		}

		static uint8_t valueof(Stance stance, bool flip)
		{
			return flip ? stance : stance + 1;
		}

		// Construct a mob by combining data from game files with
		// data sent by the server.
		Mob(int32_t oid, int32_t mobid, int8_t mode, int8_t stance, uint16_t fhid, 
			bool newspawn, int8_t team, Point<int16_t> position);

		// Draw the object.
		void draw(Point<int16_t> viewpos, float inter) const override;
		// Update movement and animations.
		int8_t update(const Physics& physics) override;

		void setcontrol(int8_t mode);

		bool isinrange(const rectangle2d<int16_t>& range) const;
		bool isalive() const;

		vector<int32_t> damage(const Attack& attack);

		// Kill the mob with the appropriate type:
		// 0: make inactive 1: death animation 2: fade out
		void kill(int8_t killtype);
		// Display the hp percentage above the mob.
		// Use the playerlevel to determine color of nametag.
		void sendhp(int8_t percentage, uint16_t playerlevel);

	private:
		struct AttackEffect
		{
			Animation hiteffect;
			Sound hitsound;
			Attack::Direction direction;
			vector<pair<int32_t, bool>> numbers;

			AttackEffect(const Attack& attack)
			{
				hiteffect = attack.hiteffect;
				hitsound = attack.hitsound;
				direction = attack.direction;
			}
		};

		static const size_t NUM_DIRECTIONS = 3;
		enum FlyDirection
		{
			STRAIGHT,
			UPWARDS,
			DOWNWARDS
		};

		static FlyDirection nextdirection(const Randomizer& randomizer)
		{
			static const FlyDirection directions[NUM_DIRECTIONS] =
			{
				STRAIGHT, UPWARDS, DOWNWARDS
			};
			size_t index = randomizer.nextint(NUM_DIRECTIONS - 1);
			return directions[index];
		}

		void applyattack(const AttackEffect& damageeffect);
		void applyknockback(bool fromright);
		void sendmovement();
		void setstance(Stance newstance);
		void nextmove();
		Point<int16_t> getheadpos(Point<int16_t> position) const;
		pair<int32_t, bool> randomdamage(double mindamage, 
			double maxdamage, float hitchance, float critical) const;

		map<Stance, Animation> animations;
		string name;
		Sound hitsound;
		Sound diesound;
		uint16_t level;
		float speed;
		float flyspeed;
		uint16_t watk;
		uint16_t matk;
		uint16_t wdef;
		uint16_t mdef;
		uint16_t accuracy;
		uint16_t avoid;
		uint16_t knockback;
		bool undead;
		bool touchdamage;
		bool noflip;
		bool notattack;
		bool canmove;
		bool canjump;
		bool canfly;

		list<pair<Bullet, AttackEffect>> bulletlist;

		EffectLayer effects;
		Text namelabel;
		MobHpBar hpbar;
		Randomizer randomizer;
		vector<DamageNumber> damagenumbers;

		uint16_t counter;

		int32_t id;
		int8_t effect;
		int8_t team;
		bool control;
		bool aggro;
		Stance stance;
		bool flip;
		FlyDirection flydirection;
		float walkforce;
		int8_t hppercent;
		bool fading;
		bool fadein;
		float alpha;
	};
}

