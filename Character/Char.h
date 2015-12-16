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
#include "Look\CharLook.h"
#include "Look\PetLook.h"
#include "Util\rectangle2d.h"
#include <memory>

namespace Character
{
	using std::unique_ptr;
	using Util::rectangle2d;
	using Gameplay::MapObject;

	// Base for characters, e.g. the player and other clients on the same map.
	class Char : public MapObject
	{
	public:
		// Player stances which determine animation and state. 
		// Values are used in movement packets (add 1 if facing left).
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
			ATTACK = 22
		};

		// Draw look, nametag, effects and chat bubble.
		void draw(vector2d<int16_t> viewpos, float inter) const override;
		// Update look and movements.
		int8_t update(const Physics& physics) override;
		// Set the position as specified
		void setposition(int16_t xpos, int16_t ypos) override;
		// Return the layer the character is currently on.
		int8_t getlayer() const override;
		// Return the unique character id.
		int32_t getoid() const override;
		// Return the current position.
		vector2d<int16_t> getposition() const override;

		// Add a pet with the specified stats.
		void addpet(uint8_t index, int32_t iid, string name, 
			int32_t uniqueid, vector2d<int16_t> pos, uint8_t stance, int32_t fhid);
		// Remove a pet with the specified index and reason.
		void removepet(uint8_t index, bool hunger);

		// Change the character's face expression by id.
		void sendface(int32_t expression);
		// Set if the character sprite is mirrored (true = facing left)
		virtual void setflip(bool flipped);
		// Change the character's stance.
		virtual void setstance(Stance newstance);

		// Return if the character is facing left.
		bool getflip() const;
		// Return the character's collision mask.
		rectangle2d<int16_t> getbounds() const;

		// Return if the char is in the Char::SIT state.
		bool issitting() const;
		// Return if the char is in the Char::LADDER or Char::ROPE state.
		bool isclimbing() const;

		// Obtain a reference to the character's sprite.
		CharLook& getlook();
		// Obtain a reference to the character's physics.
		PhysicsObject& getphobj();

	protected:
		CharLook look;
		Textlabel namelabel;
		PetLook pets[3];
		PhysicsObject phobj;

		int32_t cid;
		Stance stance;
		bool flip;
	};
}