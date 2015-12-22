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

	class PlayerState
	{
	public:
		virtual ~PlayerState(){}
		virtual void sendaction(Player& player, Keyboard::Keyaction keycode, bool pressed) const = 0;
		virtual void update(Player& player) const = 0;
		virtual void nextstate(Player& player) const = 0;
	};

	class PlayerNullState : public PlayerState
	{
	public:
		void sendaction(Player&, Keyboard::Keyaction, bool) const override {}

		void update(Player&) const override {}

		void nextstate(Player& player) const override
		{
			Char::Stance stance;
			if (player.getphobj().onground)
			{
				if (player.keydown(Keyboard::KA_LEFT))
				{
					stance = Char::WALK;
					player.setflip(false);
				}
				else if (player.keydown(Keyboard::KA_RIGHT))
				{
					stance = Char::WALK;
					player.setflip(true);
				}
				else if (player.keydown(Keyboard::KA_DOWN))
				{
					stance = Char::PRONE;
				}
				else
				{
					stance = Char::STAND;
				}
			}
			else
			{
				PhysicsObject::PhType phtype = player.getphobj().type;
				if (phtype == PhysicsObject::CLIMBING)
				{
					const Ladder* ladder = player.getladder();
					if (ladder)
					{
						stance = ladder->ladder ? Char::LADDER : Char::ROPE;
					}
					else
					{
						stance = Char::FALL;
						player.getphobj().type = PhysicsObject::NORMAL;
					}
				}
				else if (phtype == PhysicsObject::FLYING || phtype == PhysicsObject::SWIMMING)
				{
					stance = Char::SWIM;
				}
				else
				{
					stance = Char::FALL;
				}
			}
			player.setstance(stance);
		}
	};

	class PlayerStandState : public PlayerState
	{
		void sendaction(Player& player, Keyboard::Keyaction ka, bool down) const override
		{
			if (down)
			{
				switch (ka)
				{
				case Keyboard::KA_LEFT:
					player.setflip(false);
					player.setstance(Char::WALK);
					break;
				case Keyboard::KA_RIGHT:
					player.setflip(true);
					player.setstance(Char::WALK);
					break;
				case Keyboard::KA_JUMP:
					player.getphobj().vforce = -player.getjforce();
					break;
				case Keyboard::KA_DOWN:
					player.setstance(Char::PRONE);
					break;
				}
			}
		}

		void update(Player&) const override {}

		void nextstate(Player& player) const override
		{
			if (!player.getphobj().onground)
			{
				player.setstance(Char::FALL);
			}
		}
	};

	class PlayerWalkState : public PlayerState
	{
		void sendaction(Player& player, Keyboard::Keyaction ka, bool down) const override
		{
			if (down)
			{
				switch (ka)
				{
				case Keyboard::KA_LEFT:
					player.setflip(false);
					break;
				case Keyboard::KA_RIGHT:
					player.setflip(true);
					break;
				case Keyboard::KA_JUMP:
					player.getphobj().vforce = -player.getjforce();
					break;
				case Keyboard::KA_DOWN:
					player.getphobj().hspeed = 0.0f;
					player.setstance(Char::PRONE);
					break;
				}
			}
		}

		void update(Player& player) const override
		{
			if (player.isattacking())
				return;

			if (player.keydown(Keyboard::KA_LEFT))
				player.getphobj().hforce = -player.getwforce();
			else if (player.keydown(Keyboard::KA_RIGHT))
				player.getphobj().hforce = player.getwforce();
		}

		void nextstate(Player& player) const override
		{
			if (player.getphobj().onground)
			{
				if (player.getphobj().hspeed == 0.0f)
					player.setstance(Char::STAND);
			}
			else
			{
				player.setstance(Char::FALL);
			}
		}
	};

	class PlayerFallState : public PlayerState
	{
		void sendaction(Player&, Keyboard::Keyaction, bool) const override {}

		void update(Player&) const override {}

		void nextstate(Player& player) const override
		{
			if (!player.getphobj().onground)
				return;

			if (player.keydown(Keyboard::KA_LEFT))
				player.setflip(false);
			else if (player.keydown(Keyboard::KA_RIGHT))
				player.setflip(true);

			if (player.getphobj().hspeed != 0.0f)
				player.setstance(Char::WALK);
			else
				player.setstance(Char::STAND);
		}
	};

	class PlayerProneState : public PlayerState
	{
		void sendaction(Player& player, Keyboard::Keyaction ka, bool down) const override
		{
			if (down)
			{
				switch (ka)
				{
				case Keyboard::KA_JUMP:
					player.setstance(Char::STAND);
					player.sendaction(ka, down);
					break;
				}
			}
			else
			{
				switch (ka)
				{
				case Keyboard::KA_DOWN:
					player.setstance(Char::STAND);
					break;
				}
			}
		}

		void update(Player&) const override {}

		void nextstate(Player&) const override {}
	};

	class PlayerSitState : public PlayerState
	{
		void sendaction(Player& player, Keyboard::Keyaction ka, bool down) const override
		{
			if (down)
			{
				switch (ka)
				{
				case Keyboard::KA_LEFT:
					player.setflip(false);
					player.setstance(Char::WALK);
					break;
				case Keyboard::KA_RIGHT:
					player.setflip(true);
					player.setstance(Char::WALK);
					break;
				case Keyboard::KA_JUMP:
					player.setstance(Char::STAND);
					break;
				case Keyboard::KA_UP:
					player.setstance(Char::SWIM);
					break;
				}
			}
		}

		void update(Player&) const override {}

		void nextstate(Player&) const override {}
	};

	class PlayerFlyState : public PlayerState
	{
		void sendaction(Player& player, Keyboard::Keyaction ka, bool down) const override
		{
			if (down)
			{
				switch (ka)
				{
				case Keyboard::KA_LEFT:
					player.setflip(false);
					break;
				case Keyboard::KA_RIGHT:
					player.setflip(true);
					break;
				case Keyboard::KA_JUMP:
					if (abs(player.getphobj().hspeed) < 2.5f && abs(player.getphobj().vspeed) < 2.5f)
					{
						float FLYJUMPFORCE = player.getflyforce() * 20;

						player.getphobj().vforce = -FLYJUMPFORCE;
						player.getphobj().hforce = player.getflip() ? FLYJUMPFORCE : -FLYJUMPFORCE;
					}
					break;
				}
			}
		}

		void update(Player& player) const override
		{
			player.getphobj().type = PhysicsObject::FLYING;

			if (player.keydown(Keyboard::KA_LEFT))
				player.getphobj().hforce = -player.getflyforce();
			else if (player.keydown(Keyboard::KA_RIGHT))
				player.getphobj().hforce = player.getflyforce();

			if (player.keydown(Keyboard::KA_UP))
				player.getphobj().vforce = -player.getflyforce();
			else if (player.keydown(Keyboard::KA_DOWN))
				player.getphobj().vforce = player.getflyforce();
		}

		void nextstate(Player&) const override {}
	};

	class PlayerClimbState : public PlayerState
	{
	public:
		void sendaction(Player& player, Keyboard::Keyaction ka, bool down) const override
		{
			if (down)
			{
				switch (ka)
				{
				case Keyboard::KA_JUMP:
					if (player.keydown(Keyboard::KA_LEFT))
					{
						player.setflip(false);
						player.getphobj().hspeed = -player.getwforce() * 3.0f;
						player.getphobj().vforce = -player.getjforce() / 1.5f;
						cancelladder(player);
					}
					else if (player.keydown(Keyboard::KA_RIGHT))
					{
						player.setflip(true);
						player.getphobj().hspeed = player.getwforce() * 3.0f;
						player.getphobj().vforce = -player.getjforce() / 1.5f;
						cancelladder(player);
					}
					break;
				}
			}
		}

		void update(Player& player) const override
		{
			if (player.keydown(Keyboard::KA_UP))
				player.getphobj().vspeed = -player.getclimbforce();
			else if (player.keydown(Keyboard::KA_DOWN))
				player.getphobj().vspeed = player.getclimbforce();
			else
				player.getphobj().vspeed = 0.0f;
		}

		void nextstate(Player& player) const override
		{
			const Ladder* ladder = player.getladder();
			if (ladder)
			{
				double cfy;
				if (player.keydown(Keyboard::KA_DOWN))
					cfy = player.getphobj().fy;
				else
					cfy = player.getphobj().fy - 15;

				if (cfy > ladder->y2 || player.getphobj().fy + 5 < ladder->y1)
					cancelladder(player);
			}
		}

	private:
		void cancelladder(Player& player) const
		{
			player.setstance(Char::FALL);
			player.getphobj().type = PhysicsObject::NORMAL;
			player.setladder(nullptr);
		}
	};
}