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
#include "Look\CharLook.h"
#include "Look\PetLook.h"

#include "..\Gameplay\Maplemap\Mapobject.h"
#include "..\Graphics\EffectLayer.h"
#include "..\IO\Components\ChatBalloon.h"
#include "..\Util\EnumMap.h"
#include "..\Util\Rectangle.h"

namespace jrc
{
	// Base for characters, e.g. the player and other clients on the same map.
	class Char : public MapObject
	{
	public:
		// Player states which determine animation and state. 
		// Values are used in movement packets (add 1 if facing left).
		enum State : int8_t
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
			SIT = 20
		};

		static State byvalue(int8_t value)
		{
			return static_cast<State>(value);
		}

		// Character effects from Effect.wz
		enum Effect
		{
			LEVELUP,
			JOBCHANGE,
			SCROLL_SUCCESS,
			SCROLL_FAILURE,
			MONSTER_CARD,
			LENGTH
		};

		// Draw look, nametag, effects and chat bubble.
		void draw(double viewx, double viewy, float alpha) const override;
		// Update look and movements.
		int8_t update(const Physics& physics) override;
		// Return the current map layer, or 7 if on a ladder or rope.
		int8_t getlayer() const override;

		// Return the character's level.
		virtual uint16_t getlevel() const = 0;
		// Return the character's level.
		virtual int32_t getskilllevel(int32_t skillid) const = 0;
		// Return the character's attacking speed.
		virtual float getattackspeed() const = 0;
		// Return the delay until applying an attack.
		uint16_t getattackdelay(size_t no, uint8_t speed) const;

		// Set if the character sprite is mirrored (true = facing left)
		virtual void setflip(bool flipped);
		// Change the character's state.
		virtual void setstate(State newstate);
		// Change the character's stance to an attack action.
		void attack(const std::string& action);
		// Change the character's stance to an attack stance.
		void attack(Stance::Value stance);
		// Change the character's stance to it's regular attack.
		void attack(bool degenerate);

		// Display an animation as an effect with the character.
		void showeffect(Animation animation, int8_t z);
		// Display an animation as an effect ontop of the character.
		void showeffectbyid(Effect toshow);
		// Display a chat bubble with the specified line in it.
		void speak(const std::string& line);
		// Change a part of the character's look.
		void changelook(Maplestat::Value stat, int32_t id);
		// Change the character's state by id.
		void setstate(uint8_t statebyte);
		// Change the character's face expression by id.
		void sendface(int32_t expression);

		// Add a pet with the specified stats.
		void addpet(uint8_t index, int32_t iid, const std::string& name,
			int32_t uniqueid, Point<int16_t> pos, uint8_t stance, int32_t fhid);
		// Remove a pet with the specified index and reason.
		void removepet(uint8_t index, bool hunger);

		// Return if the character is facing left.
		bool getflip() const;
		// Return the name of this character.
		std::string getname() const;

		// Return if the char is in the Char::SIT state.
		bool issitting() const;
		// Return if the char is in the Char::LADDER or Char::ROPE state.
		bool isclimbing() const;
		// Return wether the character sprite uses stances for two-handed weapons.
		bool istwohanded() const;

		// Obtain a reference to this character's look.
		CharLook& getlook();
		// Obtain a const reference to this character's look.
		const CharLook& getlook() const;

		// Initialize character effects.
		static void init();

	protected:
		Char(int32_t oid, const CharLook& look, const std::string& name);

		// Update the character's animation with the given speed.
		bool update(const Physics& physics, float speed);
		// Get a speed modifier for the current stance.
		float getstancespeed() const;

		CharLook look;
		ChatBalloon chatballoon;
		EffectLayer effects;
		Text namelabel;
		PetLook pets[3];

		State state;
		bool attacking;
		bool flip;

	private:
		static EnumMap<Effect, Animation> effectdata;
	};
}