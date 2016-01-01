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
#include "Char.h"
#include "CharStats.h"
#include "Skillbook.h"
#include "Questlog.h"
#include "Telerock.h"
#include "Monsterbook.h"
#include "Buff.h"
#include "Look\CharLook.h"
#include "Inventory\Inventory.h"

#include "Gameplay\Playable.h"
#include "Gameplay\Physics\Physics.h"
#include "Gameplay\MovementInfo.h"
#include "Gameplay\Maplemap\MapInfo.h"
#include "Gameplay\Attack.h"

namespace Character
{
	using Net::CharEntry;
	using Gameplay::Playable;
	using Gameplay::Physics;
	using Gameplay::MovementFragment;
	using Gameplay::Ladder;
	using Gameplay::Seat;
	using Gameplay::Attack;
	using IO::Keyboard;

	// A class that represents the player.
	class Player : public Playable, public Char
	{
	public:
		// Construct a player object from the given character entry.
		Player(const CharEntry& entry);
		Player();

		// Update the player's animation, physics and states.
		int8_t update(const Physics& physics) override;
		// Set flipped ignore if attacking.
		void setflip(bool flipped) override;
		// Set stance ignore if attacking.
		void setstance(Stance stance) override;

		// Respawn the player at the given position.
		void respawn(vector2d<int16_t> position);
		// Sends a Keyaction to the player's state, to apply forces, change the state and other behaviour.
		void sendaction(Keyboard::Keyaction keycode, bool pressed);
		// Recalculates the total stats from base stats, inventories and skills.
		void recalcstats(bool equipchanged);
		// Change the equipment at the specified slot and recalculate stats.
		void changecloth(int16_t slot);
		// Use the item from the player's inventory with the given id.
		void useitem(int32_t itemid);

		// Return if the player is attacking.
		bool isattacking() const;
		// Return wether the player can attack or not.
		bool canattack() const;
		// Start attacking.
		void useattack(string action);
		// Create an attack struct using the player's stats.
		Attack prepareattack();
		// Create an attack struct for a regular attack.
		Attack regularattack();

		void givebuff(Buff buff);
		void cancelbuff(Buffstat buffstat);

		// Returns the current walking force, calculated from the total ES_SPEED stat.
		float getwforce() const;
		// Returns the current jumping force, calculated from the total ES_JUMP stat.
		float getjforce() const;
		// Returns the climbing force, calculated from the total ES_SPEED stat.
		float getclimbforce() const;
		// Returns the flying force.
		float getflyforce() const;

		// Returns if a Keyaction is currently active. 
		bool keydown(Keyboard::Keyaction key) const;
		// Return a pointer to the ladder the player is on.
		const Ladder* getladder() const;

		// Change players position to the seat's position and stance to Char::SIT.
		void setseat(const Seat* seat);
		// Change players xpos to the ladder x and change stance to Char::LADDER or Char::ROPE.
		void setladder(const Ladder* ladder);

		// Obtain a reference to the player's stats.
		CharStats& getstats();
		// Obtain a reference to the player's inventory.
		Inventory& getinvent();
		// Obtain a reference to the player's skills.
		Skillbook& getskills();
		// Obtain a reference to the player's questlog.
		Questlog& getquests();
		// Obtain a reference to the player's telerock locations.
		Telerock& gettrock();
		// Obtain a reference to the player's monsterbook.
		Monsterbook& getmonsterbook();

	private:
		void updatestate(const Physics& physics);
		void writemovement();
		void updatelook();
		float getattackspeed() const;
		uint16_t getstancespeed() const;

		CharStats stats;
		Inventory inventory;
		Skillbook skillbook;
		Questlog questlog;
		Telerock telerock;
		Monsterbook monsterbook;

		map<Buffstat, Buff> buffs;

		map<Keyboard::Keyaction, bool> keysdown;

		vector<MovementFragment> movements;
		MovementFragment lastmove;
		uint16_t sendcd;

		const Ladder* ladder;

		bool attacking;
	};
}

