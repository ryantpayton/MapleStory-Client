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
#include "PlayableChar.h"

namespace Character
{
	class PlayerState
	{
	public:
		virtual ~PlayerState(){}
		virtual void sendaction(PlayableChar&, Keyaction, bool) const = 0;
		virtual void update(PlayableChar&) const = 0;
		virtual void nextstate(PlayableChar&) const = 0;
	};

	class PlayerStandState : public PlayerState
	{
		void sendaction(PlayableChar& player, Keyaction ka, bool down) const override
		{
			if (down)
			{
				switch (ka)
				{
				case IO::KA_LEFT:
					player.setflip(false);
					player.setstance(Char::WALK);
					break;
				case IO::KA_RIGHT:
					player.setflip(true);
					player.setstance(Char::WALK);
					break;
				case IO::KA_JUMP:
					player.getphobj().vforce = -player.getjforce();
					break;
				case IO::KA_DOWN:
					player.setstance(Char::PRONE);
					break;
				}
			}
		}

		void update(PlayableChar&) const override {}

		void nextstate(PlayableChar& player) const override
		{
			if (!player.getphobj().onground)
			{
				player.setstance(Char::FALL);
			}
		}
	};

	class PlayerWalkState : public PlayerState
	{
		void sendaction(PlayableChar& player, Keyaction ka, bool down) const override
		{
			if (down)
			{
				switch (ka)
				{
				case IO::KA_LEFT:
					player.setflip(false);
					break;
				case IO::KA_RIGHT:
					player.setflip(true);
					break;
				case IO::KA_JUMP:
					player.getphobj().vforce = -player.getjforce();
					break;
				case IO::KA_DOWN:
					player.getphobj().hspeed = 0.0f;
					player.setstance(Char::PRONE);
					break;
				}
			}
		}

		void update(PlayableChar& player) const override
		{
			if (player.keydown(IO::KA_LEFT))
			{
				player.getphobj().hforce = -player.getwforce();
			}
			else if (player.keydown(IO::KA_RIGHT))
			{
				player.getphobj().hforce = player.getwforce();
			}
		}

		void nextstate(PlayableChar& player) const override
		{
			if (player.getphobj().onground)
			{
				if (player.getphobj().hspeed == 0.0f)
				{
					player.setstance(Char::STAND);
				}
			}
			else
			{
				player.setstance(Char::FALL);
			}
		}
	};

	class PlayerFallState : public PlayerState
	{
		void sendaction(PlayableChar& player, Keyaction ka, bool down) const override
		{

		}

		void update(PlayableChar&) const override {}

		void nextstate(PlayableChar& player) const override
		{
			if (player.getphobj().onground)
			{
				if (player.keydown(IO::KA_LEFT))
				{
					player.setflip(false);
				}
				else if (player.keydown(IO::KA_RIGHT))
				{
					player.setflip(true);
				}

				if (player.getphobj().hspeed != 0.0f)
				{
					player.setstance(Char::WALK);
				}
				else
				{
					player.setstance(Char::STAND);
				}
			}
		}
	};

	class PlayerProneState : public PlayerState
	{
		void sendaction(PlayableChar& player, Keyaction ka, bool down) const override
		{
			if (down)
			{
				switch (ka)
				{
				case IO::KA_JUMP:
					player.setstance(Char::STAND);
					player.sendaction(ka, down);
					break;
				}
			}
			else
			{
				switch (ka)
				{
				case IO::KA_DOWN:
					player.setstance(Char::STAND);
					break;
				}
			}
		}

		void update(PlayableChar&) const override {}

		void nextstate(PlayableChar&) const override {}
	};

	class PlayerSitState : public PlayerState
	{
		void sendaction(PlayableChar& player, Keyaction ka, bool down) const override
		{
			if (down)
			{
				switch (ka)
				{
				case IO::KA_LEFT:
					player.setflip(false);
					player.setstance(Char::WALK);
					break;
				case IO::KA_RIGHT:
					player.setflip(true);
					player.setstance(Char::WALK);
					break;
				case IO::KA_JUMP:
					player.setstance(Char::STAND);
					break;
				case IO::KA_UP:
					player.setstance(Char::SWIM);
					break;
				}
			}
		}

		void update(PlayableChar&) const override {}

		void nextstate(PlayableChar&) const override {}
	};

	class PlayerFlyState : public PlayerState
	{
		void sendaction(PlayableChar& player, Keyaction ka, bool down) const override
		{
			if (down)
			{
				switch (ka)
				{
				case IO::KA_LEFT:
					player.setflip(false);
					break;
				case IO::KA_RIGHT:
					player.setflip(true);
					break;
				case IO::KA_JUMP:
					if (abs(player.getphobj().hspeed) < 2.5f && abs(player.getphobj().vspeed) < 2.5f)
					{
						float FLYJUMPFORCE = player.getflyforce() * 20;

						player.getphobj().vforce = -FLYJUMPFORCE;
						if (player.getflip())
						{
							player.getphobj().hforce = FLYJUMPFORCE;
						}
						else
						{
							player.getphobj().hforce = -FLYJUMPFORCE;
						}
					}
					break;
				}
			}
		}

		void update(PlayableChar& player) const override
		{
			player.getphobj().type = PhysicsObject::FLYING;

			if (player.keydown(IO::KA_LEFT))
			{
				player.getphobj().hforce = -player.getflyforce();
			}
			else if (player.keydown(IO::KA_RIGHT))
			{
				player.getphobj().hforce = player.getflyforce();
			}

			if (player.keydown(IO::KA_UP))
			{
				player.getphobj().vforce = -player.getflyforce();
			}
			else if (player.keydown(IO::KA_DOWN))
			{
				player.getphobj().vforce = player.getflyforce();
			}
		}

		void nextstate(PlayableChar&) const override {}
	};

	class PlayerClimbState : public PlayerState
	{
		void sendaction(PlayableChar& player, Keyaction ka, bool down) const override
		{
			if (down)
			{
				switch (ka)
				{
				case IO::KA_JUMP:
					if (player.keydown(IO::KA_LEFT))
					{
						player.setflip(false);
						player.getphobj().hforce = -player.getjforce();
						player.getphobj().vforce = -player.getjforce();
					}
					else if (player.keydown(IO::KA_RIGHT))
					{
						player.setflip(true);
						player.getphobj().hforce = player.getjforce();
						player.getphobj().vforce = player.getjforce();
					}
					break;
				}
			}
		}

		void update(PlayableChar& player) const override
		{
			if (player.keydown(IO::KA_UP))
			{
				player.getphobj().vforce = -player.getclimbforce();
			}
			else if (player.keydown(IO::KA_DOWN))
			{
				player.getphobj().vforce = player.getclimbforce();
			}
		}

		void nextstate(PlayableChar& player) const override
		{
			if (player.getphobj().onground)
			{
				player.setstance(Char::STAND);
				player.getphobj().canfall = true;
			}
			else
			{
				const Ladder* ladder = player.getladder();
				if (ladder)
				{
					float cfy;
					if (player.getphobj().vspeed > 0.0f)
						cfy = player.getphobj().fy;
					else
						cfy = player.getphobj().fy - 35;

					if (cfy > ladder->y2 || cfy < ladder->y1)
					{
						player.setstance(Char::FALL);
						player.getphobj().canfall = true;
					}
				}
			}
		}
	};
}