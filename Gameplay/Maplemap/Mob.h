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
#include "Graphics\Text.h"
#include "Graphics\EffectLayer.h"
#include "Audio\Sound.h"
#include "IO\Components\DamageNumber.h"
#include "Util\rectangle2d.h"
#include "Util\Randomizer.h"
#include "Util\HashList.h"

namespace Gameplay
{
	using std::string;
	using std::pair;
	using std::vector;
	using std::list;
	using std::map;
	using Graphics::Text;
	using Graphics::EffectLayer;
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

		// Construct a mob by combining data from game files with
		// data sent by the server.
		Mob(int32_t oid, int32_t mobid, bool control, int8_t stance, uint16_t fhid, 
			bool newspawn, int8_t team, vector2d<int16_t> position);

		// Update movement and animations.
		int8_t update(const Physics& physics) override;
		// Draw the object.
		void draw(const Camera& camera, float inter) const override;

		bool isinrange(const rectangle2d<int16_t>& range) const;
		bool isactive() const override;

		vector<int32_t> damage(const Attack& attack);

		// Kill the mob with the appropriate type:
		// 0: make inactive 1: death animation 2: fade out
		void kill(int8_t killtype);
		// Display the hp percentage above the mob.
		// Use the playerlevel to determine color of nametag.
		void sendhp(int8_t percentage, uint16_t playerlevel);

	private:
		struct DamageEffect
		{
			Animation hiteffect;
			Sound hitsound;
			vector<pair<int32_t, bool>> numbers;
			bool fromright;
		};

		struct Bullet
		{
			Animation animation;
			DamageEffect damageeffect;
			vector2d<int16_t> position;

			void draw(vector2d<int16_t> viewpos, float inter) const
			{
				vector2d<int16_t> bulletpos = position + viewpos;
				bulletpos.shifty(-26);
				animation.draw(bulletpos, inter);
			}

			bool update(vector2d<int16_t> target)
			{
				animation.update();

				vector2d<int16_t> distance = target - position;
				position += distance / 10;
				return distance.length() < 10;
			}
		};

		void applydamage(const DamageEffect& damageeffect);
		void applyknockback(bool fromright);
		void writemovement();
		void parsestance(Stance toparse, node source);
		void setstance(Stance newstance);
		void nextmove();
		pair<int32_t, bool> randomdamage(int32_t mindamage, 
			int32_t maxdamage, float hitchance, float critical) const;

		map<Stance, Animation> animations;
		map<Stance, rectangle2d<int16_t>> bounds;
		string name;
		Sound hitsound;
		Sound diesound;
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
		bool noflip;
		bool notattack;

		HashList<Bullet> bulletlist;

		EffectLayer effects;
		Text namelabel;
		Randomizer randomizer;
		vector<DamageNumber> damagenumbers;

		uint16_t counter;
		vector<MovementFragment> movements;

		int32_t id;
		int8_t effect;
		int8_t team;
		bool control;
		Stance stance;
		bool flip;
		float walkforce;
		int8_t hppercent;
		bool fading;
		bool fadein;
		float alpha;
	};
}

