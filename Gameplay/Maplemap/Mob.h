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
#include "Audio\Sound.h"
#include "Gameplay\Attack.h"
#include "Gameplay\Bullet.h"
#include "Gameplay\Physics\PhysicsObject.h"
#include "Graphics\Text.h"
#include "Graphics\EffectLayer.h"
#include "Graphics\Geometry\MobHpBar.h"
#include "IO\Components\DamageNumber.h"
#include "Util\rectangle2d.h"
#include "Util\Enum.h"
#include "Util\Interpolated.h"
#include "Util\Randomizer.h"
#include "Util\TimedBool.h"
#include <list>
#include <queue>
#include <deque>

namespace Gameplay
{
	using std::string;
	using std::pair;
	using std::queue;
	using std::deque;
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

		// Draw the mob.
		void draw(Point<int16_t> viewpos, float inter) const override;
		// Draw effects on the top layer.
		void draweffects(Point<int16_t> viewpos, float inter) const;
		// Update movement and animations.
		int8_t update(const Physics& physics) override;

		// Change this mob's control mode:
		// 0 - no control, 1 - control, 2 - aggro
		void setcontrol(int8_t mode);
		// Kill the mob with the appropriate type:
		// 0 - make inactive 1 - death animation 2 - fade out
		void kill(int8_t killtype);
		// Display the hp percentage above the mob.
		// Use the playerlevel to determine color of nametag.
		void sendhp(int8_t percentage, uint16_t playerlevel);
		// Check if this mob collides with the specified rectangle.

		bool isinrange(const rectangle2d<int16_t>& range) const;
		// Check if this mob is still alive.
		bool isalive() const;
		// Damage the mob with an attack and return all damage lines.
		vector<int32_t> damage(const Attack& attack);

	private:
		struct SingleEffect
		{
			DamageNumber number;
			Animation hiteffect;
			Sound hitsound;
			uint16_t delay;

			SingleEffect(DamageNumber n, Animation he, Sound hs, uint16_t d)
			{
				number = n;
				hiteffect = he;
				hitsound = hs;
				delay = d;
			}
		};

		class AttackEffect
		{
		public:
			AttackEffect(const Attack& attack);

			void push_back(pair<int32_t, bool> number);

			bool toleft() const;
			int32_t gettotal() const;
			queue<SingleEffect> seperate(Point<int16_t> head) const;

		private:
			Animation hiteffect;
			Sound hitsound;
			Attack::Direction direction;
			vector<uint16_t> hitdelays;
			uint16_t firstdelay;

			vector<pair<int32_t, bool>> numbers;
			int32_t total;
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

		void applysingle(const SingleEffect& singleeffect);
		void applyattack(const AttackEffect& damageeffect);
		void applyknockback(bool fromright);
		void sendmovement();
		void setstance(Stance newstance);
		void nextmove();
		float calchitchance(int16_t leveldelta, int32_t accuracy) const;
		double calcmindamage(int16_t leveldelta, double mindamage) const;
		double calcmaxdamage(int16_t leveldelta, double maxdamage) const;
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

		list<SingleEffect> singlelist;
		list<pair<Bullet, AttackEffect>> bulletlist;

		EffectLayer effects;
		Text namelabel;
		MobHpBar hpbar;
		Randomizer randomizer;
		deque<DamageNumber> damagenumbers;

		TimedBool showhp;
		uint16_t counter;

		int32_t id;
		int8_t effect;
		int8_t team;
		bool dying;
		bool dead;
		bool control;
		bool aggro;
		Stance stance;
		bool flip;
		FlyDirection flydirection;
		float walkforce;
		int8_t hppercent;
		bool fading;
		bool fadein;
		Linear<float> opacity;
	};
}

