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
#include "Character\Charstats.h"
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
	using::Gameplay::Physics;
	using::Gameplay::MovementInfo;
	using::Gameplay::MovementFragment;
	using::Gameplay::Ladder;
	using::Gameplay::Seat;

	// A class that represents the player.
	class Player : public PlayableChar
	{
	public:
		// Construct a player object with the given id, look and stats. Also initializes nametag, flip and stance.
		Player(int32_t, const LookEntry&, const StatsEntry&);
		// Empty destructor.
		Player();
		// Respawn the player at the given position.
		void respawn(vector2d<int32_t>);
		// Sends a Keyaction to the player's state, to apply forces, change the state and other behaviour.
		// Parameters: Keyaction(maple key-code), bool(is key down)
		void sendaction(Keyaction, bool);
		// Can be called after sending player movement to the server to start a new stack.
		void clearmovement();
		// Recalculates the total stats from base stats, inventories and skills.
		// Parameters: bool(also recalculate inventory stats)
		void recalcstats(bool);
		// Update the player's animation, physics and states.
		// Parameters: Physics&(engine to use)
		int8_t update(const Physics&);

		void setseat(const Seat*);
		void setladder(const Ladder*);

		bool issitting() const;
		bool isclimbing() const;
		// Returns the current walking force, calculated from the total ES_SPEED stat.
		float getwforce() const;
		// Returns the current jumping force, calculated from the total ES_JUMP stat.
		float getjforce() const;
		float getclimbforce() const;
		// Returns if the player is flipped, eg. facing right.
		float getflyforce() const;
		// Returns if a Keyaction is currently active. Used by player states for some behaviour (ex. jumpdown).
		bool keydown(Keyaction) const;

		// References to various player properties.
		Charstats& getstats();
		Inventory& getinvent();
		Skillbook& getskills();
		Questlog& getquests();
		Telerock& gettrock();
		Monsterbook& getmonsterbook();

		// Obtain a reference to this object's movement info so it can be sent to the server.
		const MovementInfo& getmovement() const;

		const Ladder* getladder() const;
	private:
		Charstats stats;
		Inventory inventory;
		Skillbook skillbook;
		Questlog questlog;
		Telerock telerock;
		Monsterbook monsterbook;

		map<Keyaction, bool> keysdown;
		MovementInfo movementinfo;
		MovementFragment lastmove;

		const Ladder* ladder;
	};
}

