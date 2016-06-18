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
#include "PlayerStates.h"

#include "..\Audio\Audio.h"

namespace jrc
{
	// Base class
	void PlayerState::play_jumpsound() const
	{
		Sound(Sound::JUMP).play();
	}


	// Null state
	void PlayerNullState::update_state(Player& player) const
	{
		Char::State state;
		if (player.get_phobj().onground)
		{
			if (player.is_key_down(Keyboard::LEFT))
			{
				state = Char::WALK;
				player.set_direction(false);
			}
			else if (player.is_key_down(Keyboard::RIGHT))
			{
				state = Char::WALK;
				player.set_direction(true);
			}
			else if (player.is_key_down(Keyboard::DOWN))
			{
				state = Char::PRONE;
			}
			else
			{
				state = Char::STAND;
			}
		}
		else
		{
			Optional<const Ladder> ladder = player.get_ladder();
			if (ladder)
			{
				state = ladder->is_ladder() ? Char::LADDER : Char::ROPE;
			}
			else
			{
				state = Char::FALL;
			}
		}

		player.get_phobj().clear_flags();

		player.set_state(state);
	}


	// Standing
	void PlayerStandState::initialize(Player& player) const
	{
		player.get_phobj().type = PhysicsObject::NORMAL;
	}

	void PlayerStandState::send_action(Player& player, Keyboard::Action ka, bool down) const
	{
		if (player.is_attacking())
			return;

		if (down)
		{
			switch (ka)
			{
			case Keyboard::LEFT:
				player.set_direction(false);
				player.set_state(Char::WALK);
				break;
			case Keyboard::RIGHT:
				player.set_direction(true);
				player.set_state(Char::WALK);
				break;
			case Keyboard::JUMP:
				play_jumpsound();
				player.get_phobj().vforce = -player.get_jumpforce();
				break;
			case Keyboard::DOWN:
				player.set_state(Char::PRONE);
				break;
			}
		}
	}

	void PlayerStandState::update(Player& player) const
	{
		if (player.get_phobj().enablejd == false)
			player.get_phobj().set_flag(PhysicsObject::CHECKBELOW);
	}

	void PlayerStandState::update_state(Player& player) const
	{
		if (!player.get_phobj().onground)
		{
			player.set_state(Char::FALL);
		}
	}


	// Walking
	void PlayerWalkState::initialize(Player& player) const
	{
		player.get_phobj().type = PhysicsObject::NORMAL;
	}

	void PlayerWalkState::send_action(Player& player, Keyboard::Action ka, bool down) const
	{
		if (player.is_attacking())
			return;

		if (down)
		{
			switch (ka)
			{
			case Keyboard::LEFT:
				player.set_direction(false);
				break;
			case Keyboard::RIGHT:
				player.set_direction(true);
				break;
			case Keyboard::JUMP:
				play_jumpsound();
				player.get_phobj().vforce = -player.get_jumpforce();
				break;
			case Keyboard::DOWN:
				player.set_state(Char::PRONE);
				break;
			}
		}
	}

	bool PlayerWalkState::haswalkinput(const Player& player) const
	{
		return player.is_key_down(Keyboard::LEFT) || player.is_key_down(Keyboard::RIGHT);
	}

	void PlayerWalkState::update(Player& player) const
	{
		if (!player.is_attacking() && haswalkinput(player))
			player.get_phobj().hforce += player.getflip() ? player.get_walkforce() : -player.get_walkforce();

		if (player.get_phobj().enablejd == false)
			player.get_phobj().set_flag(PhysicsObject::CHECKBELOW);
	}

	void PlayerWalkState::update_state(Player& player) const
	{
		if (player.get_phobj().onground)
		{
			if (!haswalkinput(player) || player.get_phobj().hspeed == 0.0f)
				player.set_state(Char::STAND);
		}
		else
		{
			player.set_state(Char::FALL);
		}
	}

	// Falling
	void PlayerFallState::initialize(Player& player) const
	{
		player.get_phobj().type = PhysicsObject::NORMAL;
	}

	void PlayerFallState::update_state(Player& player) const
	{
		if (player.get_phobj().onground)
		{
			if (player.is_key_down(Keyboard::LEFT))
			{
				player.set_direction(false);
				player.set_state(Char::WALK);
			}
			else if (player.is_key_down(Keyboard::RIGHT))
			{
				player.set_direction(true);
				player.set_state(Char::WALK);
			}
			else
			{
				player.set_state(Char::STAND);
			}
		}
		else if (player.is_underwater())
		{
			player.set_state(Char::SWIM);
		}
	}


	// Prone
	void PlayerProneState::send_action(Player& player, Keyboard::Action ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case Keyboard::JUMP:
				if (player.get_phobj().enablejd && player.is_key_down(Keyboard::DOWN))
				{
					play_jumpsound();
					player.get_phobj().y = player.get_phobj().groundbelow;
					player.set_state(Char::FALL);
				}
				else
				{
					player.set_state(Char::STAND);
					player.send_action(ka, down);
				}
				break;
			}
		}
		else
		{
			switch (ka)
			{
			case Keyboard::DOWN:
				player.set_state(Char::STAND);
				break;
			}
		}
	}

	void PlayerProneState::update(Player& player) const
	{
		if (player.get_phobj().enablejd == false)
			player.get_phobj().set_flag(PhysicsObject::CHECKBELOW);
	}


	// Sitting
	void PlayerSitState::send_action(Player& player, Keyboard::Action ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case Keyboard::LEFT:
				player.set_direction(false);
				player.set_state(Char::WALK);
				break;
			case Keyboard::RIGHT:
				player.set_direction(true);
				player.set_state(Char::WALK);
				break;
			case Keyboard::JUMP:
				play_jumpsound();
				player.set_state(Char::STAND);
				break;
			case Keyboard::UP:
				player.set_state(Char::SWIM);
				break;
			}
		}
	}


	// Flying
	void PlayerFlyState::initialize(Player& player) const
	{
		player.get_phobj().type = player.is_underwater() ? PhysicsObject::SWIMMING : PhysicsObject::FLYING;
	}

	void PlayerFlyState::send_action(Player& player, Keyboard::Action ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case Keyboard::LEFT:
				player.set_direction(false);
				break;
			case Keyboard::RIGHT:
				player.set_direction(true);
				break;
			}
		}
	}

	void PlayerFlyState::update(Player& player) const
	{
		if (player.is_attacking())
			return;

		if (player.is_key_down(Keyboard::LEFT))
			player.get_phobj().hforce = -player.get_flyforce();
		else if (player.is_key_down(Keyboard::RIGHT))
			player.get_phobj().hforce = player.get_flyforce();

		if (player.is_key_down(Keyboard::UP))
			player.get_phobj().vforce = -player.get_flyforce();
		else if (player.is_key_down(Keyboard::DOWN))
			player.get_phobj().vforce = player.get_flyforce();
	}

	void PlayerFlyState::update_state(Player& player) const
	{
		if (player.get_phobj().onground && player.is_underwater())
		{
			Char::State state;
			if (player.is_key_down(Keyboard::LEFT))
			{
				state = Char::WALK;
				player.set_direction(false);
			}
			else if (player.is_key_down(Keyboard::RIGHT))
			{
				state = Char::WALK;
				player.set_direction(true);
			}
			else if (player.is_key_down(Keyboard::DOWN))
			{
				state = Char::PRONE;
			}
			else
			{
				state = Char::STAND;
			}
			player.set_state(state);
		}
	}


	// Climbing
	void PlayerClimbState::initialize(Player& player) const
	{
		player.get_phobj().type = PhysicsObject::FIXATED;
	}

	void PlayerClimbState::send_action(Player& player, Keyboard::Action ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case Keyboard::JUMP:
				if (player.is_key_down(Keyboard::LEFT))
				{
					play_jumpsound();
					player.set_direction(false);
					player.get_phobj().hspeed = -player.get_walkforce() * 8.0;
					player.get_phobj().vspeed = -player.get_jumpforce() / 1.5;
					cancel_ladder(player);
				}
				else if (player.is_key_down(Keyboard::RIGHT))
				{
					play_jumpsound();
					player.set_direction(true);
					player.get_phobj().hspeed = player.get_walkforce() * 8.0;
					player.get_phobj().vspeed = -player.get_jumpforce() / 1.5;
					cancel_ladder(player);
				}
				break;
			}
		}
	}

	void PlayerClimbState::update(Player& player) const
	{
		if (player.is_key_down(Keyboard::UP))
		{
			player.get_phobj().vspeed = -player.get_climbforce();
		}
		else if (player.is_key_down(Keyboard::DOWN))
		{
			player.get_phobj().vspeed = player.get_climbforce();
		}
		else
		{
			player.get_phobj().vspeed = 0.0;
		}
	}

	void PlayerClimbState::update_state(Player& player) const
	{
		int16_t y = player.get_phobj().get_y();
		bool downwards = player.is_key_down(Keyboard::DOWN);
		bool felloff = player.get_ladder()
			.maporfalse(&Ladder::felloff, y, downwards);
		if (felloff)
		{
			cancel_ladder(player);
		}
	}

	void PlayerClimbState::cancel_ladder(Player& player) const
	{
		player.set_state(Char::FALL);
		player.set_ladder(nullptr);
	}
}
