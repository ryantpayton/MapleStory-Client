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
#include "PlayerState.h"

namespace Character
{
	class PlayerStandState : public PlayerState
	{
		void sendaction(PlayableChar& player, Keyaction ka, bool down) const
		{
			if (down)
			{
				switch (ka)
				{
				case IO::KA_LEFT:
					player.setflip(false);
					player.setstance(PST_WALK);
					break;
				case IO::KA_RIGHT:
					player.setflip(true);
					player.setstance(PST_WALK);
					break;
				case IO::KA_JUMP:
					player.getphobj().vforce = -player.getjforce();
					break;
				case IO::KA_DOWN:
					player.setstance(PST_PRONE);
					break;
				}
			}
		}

		void update(PlayableChar&) const {}

		void nextstate(PlayableChar& player) const
		{
			if (!player.getphobj().onground)
			{
				player.setstance(PST_FALL);
			}
		}
	};

	class PlayerWalkState : public PlayerState
	{
		void sendaction(PlayableChar& player, Keyaction ka, bool down) const
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
					player.setstance(PST_PRONE);
					break;
				}
			}
		}

		void update(PlayableChar& player) const
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

		void nextstate(PlayableChar& player) const
		{
			if (player.getphobj().onground)
			{
				if (player.getphobj().hspeed == 0.0f)
				{
					player.setstance(PST_STAND);
				}
			}
			else
			{
				player.setstance(PST_FALL);
			}
		}
	};

	class PlayerFallState : public PlayerState
	{
		void sendaction(PlayableChar& player, Keyaction ka, bool down) const
		{

		}

		void update(PlayableChar&) const {}

		void nextstate(PlayableChar& player) const
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
					player.setstance(PST_WALK);
				}
				else
				{
					player.setstance(PST_STAND);
				}
			}
		}
	};

	class PlayerProneState : public PlayerState
	{
		void sendaction(PlayableChar& player, Keyaction ka, bool down) const
		{
			if (down)
			{
				switch (ka)
				{
				case IO::KA_JUMP:
					player.setstance(PST_STAND);
					player.sendaction(ka, down);
					break;
				}
			}
			else
			{
				switch (ka)
				{
				case IO::KA_DOWN:
					player.setstance(PST_STAND);
					break;
				}
			}
		}

		void update(PlayableChar&) const {}

		void nextstate(PlayableChar&) const {}
	};

	class PlayerSitState : public PlayerState
	{
		void sendaction(PlayableChar& player, Keyaction ka, bool down) const
		{
			if (down)
			{
				switch (ka)
				{
				case IO::KA_LEFT:
					player.setflip(false);
					player.setstance(PST_WALK);
					break;
				case IO::KA_RIGHT:
					player.setflip(true);
					player.setstance(PST_WALK);
					break;
				case IO::KA_JUMP:
					player.setstance(PST_STAND);
					break;
				}
			}
		}

		void update(PlayableChar&) const {}

		void nextstate(PlayableChar&) const {}
	};
}