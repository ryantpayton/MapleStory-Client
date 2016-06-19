/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "Look\Afterimage.h"
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
		int8_t get_layer() const override;

		// Return the character's level.
		virtual uint16_t get_level() const = 0;
		// Return the character's level.
		virtual int32_t get_skilllevel(int32_t skillid) const = 0;
		// Return the character's base attacking speed.
		virtual int8_t get_base_attackspeed() const = 0;

		// Return the attack speed as an integer.
		int8_t get_integer_attackspeed() const;
		// Return the attack speed as a multiplier.
		float get_real_attackspeed() const;
		// Return the delay until applying an attack.
		uint16_t get_attackdelay(size_t no) const;

		// Set if the character sprite is mirrored (true = facing left)
		virtual void set_direction(bool flipped);
		// Change the character's state.
		virtual void set_state(State newstate);
		// Change the character's stance to an attack action.
		void attack(const std::string& action);
		// Change the character's stance to an attack stance.
		void attack(Stance::Value stance);
		// Change the character's stance to it's regular attack.
		void attack(bool degenerate);
		// Set the afterimage for an attack.
		void set_afterimage(int32_t skill_id);
		// Return the current afterimage.
		const Afterimage& get_afterimage() const;

		// Display an animation as an effect with the character.
		void show_effect(Animation animation, int8_t z);
		// Display an animation as an effect ontop of the character.
		void show_effect_id(Effect toshow);
		// Display a chat bubble with the specified line in it.
		void speak(const std::string& line);
		// Change a part of the character's look.
		void change_look(Maplestat::Value stat, int32_t id);
		// Change the character's state by id.
		void set_state(uint8_t statebyte);
		// Change the character's face expression by id.
		void set_expression(int32_t expression);

		// Add a pet with the specified stats.
		void add_pet(uint8_t index, int32_t iid, const std::string& name,
			int32_t uniqueid, Point<int16_t> pos, uint8_t stance, int32_t fhid);
		// Remove a pet with the specified index and reason.
		void remove_pet(uint8_t index, bool hunger);

		// Return if the character is facing left.
		bool getflip() const;
		// Return the name of this character.
		std::string get_name() const;

		// Return if the char is in the Char::SIT state.
		bool is_sitting() const;
		// Return if the char is in the Char::LADDER or Char::ROPE state.
		bool is_climbing() const;
		// Return wether the character sprite uses stances for two-handed weapons.
		bool is_twohanded() const;

		// Obtain a reference to this character's look.
		CharLook& get_look();
		// Obtain a const reference to this character's look.
		const CharLook& get_look() const;

		// Initialize character effects.
		static void init();

	protected:
		Char(int32_t oid, const CharLook& look, const std::string& name);

		// Update the character's animation with the given speed.
		bool update(const Physics& physics, float speed);
		// Get a speed modifier for the current stance.
		float get_stancespeed() const;

		CharLook look;
		ChatBalloon chatballoon;
		EffectLayer effects;
		Afterimage afterimage;
		Text namelabel;
		PetLook pets[3];

		State state;
		bool attacking;
		bool flip;

	private:
		static EnumMap<Effect, Animation> effectdata;
	};
}