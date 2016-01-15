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
#include "SoundFactory.h"
#include "Audioplayer.h"
#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"

namespace SoundFactory
{
	using nl::node;

	size_t soundids[NUM_SOUNDS] = {};

	void addsound(Sound sound, node src)
	{
		soundids[sound] = Audioplayer::addsound(src);
	}

	void init()
	{
		node uisrc = nl::nx::sound["UI.img"];

		addsound(MOUSESCLICK, uisrc["BtMouseClick"]);
		addsound(MOUSESHOVER, uisrc["BtMouseOver"]);
		addsound(MOUSEGRAB, uisrc["DragStart"]);
		addsound(MOUSEDROP, uisrc["DragEnd"]);
		addsound(CHARSELECT, uisrc["CharSelect"]);

		node gamesrc = nl::nx::sound["Game.img"];

		addsound(GAMESTART, gamesrc["GameIn"]);
		addsound(JUMP, gamesrc["Jump"]);
		addsound(DROP, gamesrc["DropItem"]);
		addsound(PICKUP, gamesrc["PickUpItem"]);
		addsound(PORTAL, gamesrc["Portal"]);
		addsound(LEVELUP, gamesrc["LevelUp"]);

		node wepsrc = nl::nx::sound["Weapon.img"];

		addsound(WEP_HANDS, wepsrc["barehands"]["Attack"]);
		addsound(WEP_BOW, wepsrc["bow"]["Attack2"]);
		addsound(WEP_XBOW, wepsrc["cBow"]["Attack2"]);
		addsound(WEP_GUN, wepsrc["gun"]["Attack2"]);
		addsound(WEP_KNUCKLE, wepsrc["knuckle"]["Attack"]);
		addsound(WEP_MACE, wepsrc["mace"]["Attack"]);
		addsound(WEP_POLEARM, wepsrc["poleArm"]["Attack"]);
		addsound(WEP_SPEAR, wepsrc["spear"]["Attack"]);
		addsound(WEP_LONGSWORD, wepsrc["swordL"]["Attack"]);
		addsound(WEP_SHORTSWORD, wepsrc["swordS"]["Attack"]);
		addsound(WEP_TGLOVE, wepsrc["tGlove"]["Attack2"]);
	}

	void play(Sound sound)
	{
		size_t id = soundids[sound];
		if (id > 0)
			Audioplayer::playsound(id);
	}
}