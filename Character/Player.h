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
#include "Character\PlayableChar.h"
#include "Character\CharStats.h"
#include "Character\Look\CharLook.h"
#include "Character\Inventory\Inventory.h"
#include "Character\Skillbook.h"
#include "Character\Questlog.h"
#include "Character\Telerock.h"
#include "Character\Monsterbook.h"
#include "Character\PlayerStates.h"
#include "Gameplay\Physics\Physics.h"
#include "Gameplay\MovementInfo.h"
#include "Gameplay\Maplemap\MapInfo.h"

namespace Character
{
	using Net::CharEntry;
	using Gameplay::Physics;
	using Gameplay::MovementInfo;
	using Gameplay::MovementFragment;
	using Gameplay::Ladder;
	using Gameplay::Seat;

	// A class that represents the player.
	class Player : public PlayableChar
	{
	public:
		// Construct a player object from the given char entry.
		// Also initializes nametag, and stance.
		Player(const CharEntry& entry);
		// Empty destructor.
		Player();

		// Respawn the player at the given position.
		void respawn(vector2d<int16_t> position);
		// Sends a Keyaction to the player's state, to apply forces, change the state and other behaviour.
		void sendaction(IO::Keyboard::Keyaction keycode, bool pressed);
		// Can be called after sending player movement to the server to start a new stack.
		void clearmovement();
		// Recalculates the total stats from base stats, inventories and skills.
		void recalcstats(bool equipchanged);
		// Update the player's animation, physics and states.
		int8_t update(const Physics& physics) override;

		bool tryattack();

		// Returns the current walking force, calculated from the total ES_SPEED stat.
		float getwforce() const override;
		// Returns the current jumping force, calculated from the total ES_JUMP stat.
		float getjforce() const override;
		// Returns the climbing force, calculated from the total ES_SPEED stat.
		float getclimbforce() const override;
		// Returns the flying force.
		float getflyforce() const override;

		bool isattacking() const override;
		// Returns if a Keyaction is currently active. 
		bool keydown(IO::Keyboard::Keyaction) const override;
		// Return a pointer to the ladder the player is on.
		const Ladder* getladder() const override;

		// Change players position to the seat's position and stance to Char::SIT.
		void setseat(const Seat* seat);
		// Change players xpos to the ladder x and change stance to Char::LADDER or Char::ROPE.
		void setladder(const Ladder* ladder);

		void setflip(bool flipped) override;
		void setstance(Stance stance) override;

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

		// Obtain a reference to this object's movement info so it can be sent to the server.
		const MovementInfo& getmovement() const;

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

		map<IO::Keyboard::Keyaction, bool> keysdown;
		MovementInfo movementinfo;
		MovementFragment lastmove;

		const Ladder* ladder;

		bool attacking;
	};
}

