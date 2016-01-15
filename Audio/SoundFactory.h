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

namespace SoundFactory
{
	// Enumeration of all preloaded sounds.
	const size_t NUM_SOUNDS = 29;
	enum Sound
	{
		// UI
		MOUSESCLICK,
		MOUSESHOVER,
		MOUSEGRAB,
		MOUSEDROP,
		CHARSELECT,

		// Game
		GAMESTART,
		JUMP,
		DROP,
		PICKUP,
		PORTAL,
		LEVELUP,
		DEAD,
		TRANSFORM,
		QUESTCLEAR,
		QUESTALERT,
		MONSTERCARD,
		SCROLLSUCCESS,
		SCROLLFAIL,

		// Weapons
		WEP_HANDS,
		WEP_BOW,
		WEP_XBOW,
		WEP_GUN,
		WEP_KNUCKLE,
		WEP_MACE,
		WEP_POLEARM,
		WEP_SPEAR,
		WEP_LONGSWORD,
		WEP_SHORTSWORD,
		WEP_TGLOVE
	};

	void init();

	void play(Sound sound);
}