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
#include "PlayerStates.h"
#include "Audio\Audioplayer.h"

namespace Character
{
	// Base class
	void PlayerState::playjumpsound() const
	{
		using Audio::AudioPlayer;
		AudioPlayer::get().playsound(AudioPlayer::JUMP);
	}


	// Null state
	void PlayerNullState::nextstate(Player& player) const
	{
		Char::State state;
		if (player.getphobj().onground)
		{
			if (player.keydown(Keyboard::LEFT))
			{
				state = Char::WALK;
				player.setflip(false);
			}
			else if (player.keydown(Keyboard::RIGHT))
			{
				state = Char::WALK;
				player.setflip(true);
			}
			else if (player.keydown(Keyboard::DOWN))
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
			PhysicsObject::Type phtype = player.getphobj().type;
			if (phtype == PhysicsObject::FLYING || phtype == PhysicsObject::SWIMMING)
			{
				state = Char::SWIM;
			}
			else
			{
				Optional<const Ladder> ladder = player.getladder();
				if (ladder)
				{
					state = ladder->isladder() ? Char::LADDER : Char::ROPE;
				}
				else
				{
					state = Char::FALL;
				}
			}
		}
		player.setstate(state);
	}


	// Standing
	void PlayerStandState::onentry(Player& player) const
	{
		player.getphobj().type = PhysicsObject::NORMAL;
	}

	void PlayerStandState::sendaction(Player& player, Keyboard::Action ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case Keyboard::LEFT:
				player.setflip(false);
				player.setstate(Char::WALK);
				break;
			case Keyboard::RIGHT:
				player.setflip(true);
				player.setstate(Char::WALK);
				break;
			case Keyboard::JUMP:
				playjumpsound();
				player.getphobj().vforce = -player.getjforce();
				break;
			case Keyboard::DOWN:
				player.setstate(Char::PRONE);
				break;
			}
		}
	}

	void PlayerStandState::nextstate(Player& player) const
	{
		if (!player.getphobj().onground)
		{
			player.setstate(Char::FALL);
		}
	}


	// Walking
	void PlayerWalkState::onentry(Player& player) const
	{
		player.getphobj().type = PhysicsObject::NORMAL;
	}

	void PlayerWalkState::sendaction(Player& player, Keyboard::Action ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case Keyboard::LEFT:
				player.setflip(false);
				break;
			case Keyboard::RIGHT:
				player.setflip(true);
				break;
			case Keyboard::JUMP:
				playjumpsound();
				player.getphobj().vforce = -player.getjforce();
				break;
			case Keyboard::DOWN:
				player.setstate(Char::PRONE);
				break;
			}
		}
	}

	bool PlayerWalkState::haswalkinput(const Player& player) const
	{
		return player.keydown(Keyboard::LEFT) || player.keydown(Keyboard::RIGHT);
	}

	void PlayerWalkState::update(Player& player) const
	{
		if (player.isattacking())
			return;

		if (haswalkinput(player))
			player.getphobj().hforce += player.getflip() ? player.getwforce() : -player.getwforce();
	}

	void PlayerWalkState::nextstate(Player& player) const
	{
		if (player.getphobj().onground)
		{
			if (!haswalkinput(player) || player.getphobj().hspeed == 0.0f)
				player.setstate(Char::STAND);
		}
		else
		{
			player.setstate(Char::FALL);
		}
	}

	// Falling
	void PlayerFallState::onentry(Player& player) const
	{
		player.getphobj().type = PhysicsObject::NORMAL;
	}

	void PlayerFallState::nextstate(Player& player) const
	{
		if (player.getphobj().onground)
		{
			if (player.keydown(Keyboard::LEFT))
			{
				player.setflip(false);
				player.setstate(Char::WALK);
			}
			else if (player.keydown(Keyboard::RIGHT))
			{
				player.setflip(true);
				player.setstate(Char::WALK);
			}
			else
			{
				player.setstate(Char::STAND);
			}
		}
		else if (player.isunderwater())
		{
			player.setstate(Char::SWIM);
		}
	}


	// Prone
	void PlayerProneState::sendaction(Player& player, Keyboard::Action ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case Keyboard::JUMP:
				if (player.getphobj().enablejd && player.keydown(Keyboard::DOWN))
				{
					playjumpsound();
					player.getphobj().fy = player.getphobj().groundbelow;
					player.setstate(Char::FALL);
				}
				else
				{
					player.setstate(Char::STAND);
					player.sendaction(ka, down);
				}
				break;
			}
		}
		else
		{
			switch (ka)
			{
			case Keyboard::DOWN:
				player.setstate(Char::STAND);
				break;
			}
		}
	}


	// Sitting
	void PlayerSitState::sendaction(Player& player, Keyboard::Action ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case Keyboard::LEFT:
				player.setflip(false);
				player.setstate(Char::WALK);
				break;
			case Keyboard::RIGHT:
				player.setflip(true);
				player.setstate(Char::WALK);
				break;
			case Keyboard::JUMP:
				playjumpsound();
				player.setstate(Char::STAND);
				break;
			case Keyboard::UP:
				player.setstate(Char::SWIM);
				break;
			}
		}
	}


	// Flying
	void PlayerFlyState::onentry(Player& player) const
	{
		player.getphobj().type = player.isunderwater() ? PhysicsObject::SWIMMING : PhysicsObject::FLYING;
	}

	void PlayerFlyState::sendaction(Player& player, Keyboard::Action ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case Keyboard::LEFT:
				player.setflip(false);
				break;
			case Keyboard::RIGHT:
				player.setflip(true);
				break;
			}
		}
	}

	void PlayerFlyState::update(Player& player) const
	{
		if (player.keydown(Keyboard::LEFT))
			player.getphobj().hforce = -player.getflyforce();
		else if (player.keydown(Keyboard::RIGHT))
			player.getphobj().hforce = player.getflyforce();

		if (player.keydown(Keyboard::UP))
			player.getphobj().vforce = -player.getflyforce();
		else if (player.keydown(Keyboard::DOWN))
			player.getphobj().vforce = player.getflyforce();
	}

	void PlayerFlyState::nextstate(Player& player) const
	{
		if (player.getphobj().onground && player.isunderwater())
		{
			Char::State state;
			if (player.keydown(Keyboard::LEFT))
			{
				state = Char::WALK;
				player.setflip(false);
			}
			else if (player.keydown(Keyboard::RIGHT))
			{
				state = Char::WALK;
				player.setflip(true);
			}
			else if (player.keydown(Keyboard::DOWN))
			{
				state = Char::PRONE;
			}
			else
			{
				state = Char::STAND;
			}
			player.setstate(state);
		}
	}


	// Climbing
	void PlayerClimbState::onentry(Player& player) const
	{
		player.getphobj().type = PhysicsObject::FIXATED;
	}

	void PlayerClimbState::sendaction(Player& player, Keyboard::Action ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case Keyboard::JUMP:
				if (player.keydown(Keyboard::LEFT))
				{
					playjumpsound();
					player.setflip(false);
					player.getphobj().hspeed = -player.getwforce() * 3.0f;
					player.getphobj().vforce = -player.getjforce() / 1.5f;
					cancelladder(player);
				}
				else if (player.keydown(Keyboard::RIGHT))
				{
					playjumpsound();
					player.setflip(true);
					player.getphobj().hspeed = player.getwforce() * 3.0f;
					player.getphobj().vforce = -player.getjforce() / 1.5f;
					cancelladder(player);
				}
				break;
			}
		}
	}

	void PlayerClimbState::update(Player& player) const
	{
		if (player.keydown(Keyboard::UP))
			player.getphobj().vspeed = -player.getclimbforce();
		else if (player.keydown(Keyboard::DOWN))
			player.getphobj().vspeed = player.getclimbforce();
		else
			player.getphobj().vspeed = 0.0f;
	}

	void PlayerClimbState::nextstate(Player& player) const
	{
		int16_t y = static_cast<int16_t>(player.getphobj().fy);
		bool downwards = player.keydown(Keyboard::DOWN);
		bool felloff = player.getladder()
			.maporfalse(&Ladder::felloff, y, downwards);

		if (felloff)
			cancelladder(player);
	}

	void PlayerClimbState::cancelladder(Player& player) const
	{
		player.setstate(Char::FALL);
		player.setladder(nullptr);
	}
}
