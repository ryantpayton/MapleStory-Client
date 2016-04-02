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
#include "Player.h"

namespace Character
{
	using IO::Keyboard;

	// Base class for player states
	class PlayerState
	{
	public:
		virtual ~PlayerState(){}

		// Actions taken when transitioning into the state.
		virtual void onentry(Player& player) const = 0;
		// How to handle inputs while in the state.
		virtual void sendaction(Player& player, Keyboard::Action action, bool pressed) const = 0;
		// Actions taken in the player's update method, before physics are applied.
		virtual void update(Player& player) const = 0;
		// Transition into a new state after physics have been applied.
		virtual void nextstate(Player& player) const = 0;

	protected:
		using PhysicsObject = Gameplay::PhysicsObject;

		// Play the jumping sound.
		void playjumpsound() const;
	};

	// The initial state, determines which state the player should be in.
	class PlayerNullState : public PlayerState
	{
	public:
		void onentry(Player&) const override {}
		void sendaction(Player&, Keyboard::Action, bool) const override {}
		void update(Player&) const override {}

		void nextstate(Player& player) const override;
	};

	// The standing state.
	class PlayerStandState : public PlayerState
	{
	public:
		void onentry(Player& player) const override;
		void sendaction(Player& player, Keyboard::Action ka, bool down) const override;

		void update(Player&) const override {}

		void nextstate(Player& player) const override;
	};

	// The walking state.
	class PlayerWalkState : public PlayerState
	{
		void onentry(Player& player) const override;
		void sendaction(Player& player, Keyboard::Action ka, bool down) const override;
		void update(Player& player) const override;
		void nextstate(Player& player) const override;

	private:
		bool haswalkinput(const Player& player) const;
	};

	// The falling state.
	class PlayerFallState : public PlayerState
	{
	public:
		void onentry(Player& player) const override;

		void sendaction(Player&, Keyboard::Action, bool) const override {}
		void update(Player&) const override {}

		void nextstate(Player& player) const override;
	};

	// The prone state (lying down).
	class PlayerProneState : public PlayerState
	{
	public:
		void onentry(Player&) const override {}

		void sendaction(Player& player, Keyboard::Action ka, bool down) const override;

		void update(Player&) const override {}
		void nextstate(Player&) const override {}
	};

	// The sitting state.
	class PlayerSitState : public PlayerState
	{
	public:
		void onentry(Player&) const override {}

		void sendaction(Player& player, Keyboard::Action ka, bool down) const override;

		void update(Player&) const override {}
		void nextstate(Player&) const override {}
	};

	// The flying or swimming state.
	class PlayerFlyState : public PlayerState
	{
		void onentry(Player& player) const override;
		void sendaction(Player& player, Keyboard::Action ka, bool down) const override;
		void update(Player& player) const override;
		void nextstate(Player& player) const override;
	};

	// The climbing state.
	class PlayerClimbState : public PlayerState
	{
	public:
		void onentry(Player& player) const override;
		void sendaction(Player& player, Keyboard::Action ka, bool down) const override;
		void update(Player& player) const override;
		void nextstate(Player& player) const override;

	private:
		void cancelladder(Player& player) const;
	};
}